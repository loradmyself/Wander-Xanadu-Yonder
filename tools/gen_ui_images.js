/*
 * SPDX-License-Identifier: Apache-2.0
 *
 * WXY 基础 UI 图片资源生成器
 *
 * 用途：把设计稿（内联 SVG，精确几何）渲染成 PNG，供 LVGL Image Converter
 * 转成 C 数组。图标输出为白色图形 + alpha 通道（A8 遮罩），运行期由
 * wxy_icon_create() 的 recolor 按主题令牌染色，一套图适配三主题。
 *
 * 用法：node tools/gen_ui_images.js
 * 依赖：系统 Edge/Chrome（无头模式），不需要 npm 包。
 * 输出：assets/icons/wxy_ic_<slot>_<size>.png
 *       assets/ui/wxy_vinyl_field_560.png / wxy_vinyl_label_156.png / wxy_mini_cd_56.png
 *       assets/icons/_preview.png（图标总览，仅设计检查用，不进固件）
 */

'use strict';

const fs = require('fs');
const path = require('path');
const cp = require('child_process');

const ROOT = path.resolve(__dirname, '..');
const ICON_DIR = path.join(ROOT, 'assets', 'icons');
const UI_DIR = path.join(ROOT, 'assets', 'ui');
const TMP = path.join(ROOT, '.tmp_imggen');

const EDGE_CANDIDATES = [
  'C:/Program Files (x86)/Microsoft/Edge/Application/msedge.exe',
  'C:/Program Files/Microsoft/Edge/Application/msedge.exe',
  'C:/Program Files/Google/Chrome/Application/chrome.exe'
];

/* ------------------------------------------------------------------ */
/* 图标：24x24 网格，1.7px 线宽，圆头圆角，白色 + 透明底 = A8 遮罩       */
/* ------------------------------------------------------------------ */

const line = (body) =>
  '<svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 24 24">' +
  '<g stroke="#ffffff" stroke-width="1.7" stroke-linecap="round" ' +
  'stroke-linejoin="round" fill="none">' + body + '</g></svg>';

const solid = (body) =>
  '<svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 24 24">' +
  '<g fill="#ffffff" stroke="none">' + body + '</g></svg>';

/* [文件名, 输出像素, SVG] —— 尺寸档与 slots.h 的默认档一致 */
const ICONS = [
  ['wxy_ic_mic_24', 24, line('<rect x="9" y="3" width="6" height="11" rx="3"/><path d="M5.5 11.5a6.5 6.5 0 0 0 13 0"/><path d="M12 18v3M8.5 21h7"/>')],
  ['wxy_ic_send_24', 24, line('<path d="M21 3 10.5 13.5"/><path d="M21 3 14.5 21l-4-7.5L3 9.5 21 3Z"/>')],
  ['wxy_ic_back_24', 24, line('<path d="M15 4 7 12l8 8"/>')],

  ['wxy_ic_chat_24', 24, line('<path d="M20 15a2 2 0 0 1-2 2H9l-5 4V6a2 2 0 0 1 2-2h12a2 2 0 0 1 2 2v9Z"/>')],
  ['wxy_ic_home_24', 24, line('<path d="M3 10.5 12 3l9 7.5"/><path d="M5.5 9.5V20h13V9.5"/><path d="M10 20v-6h4v6"/>')],
  ['wxy_ic_tools_24', 24, line('<path d="M4 6h16M4 12h11M4 18h16"/>')],
  ['wxy_ic_set_24', 24, line('<circle cx="12" cy="12" r="3.2"/><path d="M12 2.5v3M12 18.5v3M2.5 12h3M18.5 12h3M5.3 5.3l2.1 2.1M16.6 16.6l2.1 2.1M18.7 5.3l-2.1 2.1M7.4 16.6l-2.1 2.1"/>')],
  ['wxy_ic_eye_24', 24, line('<path d="M2.5 12S6 6.5 12 6.5 21.5 12 21.5 12 18 17.5 12 17.5 2.5 12 2.5 12Z"/><circle cx="12" cy="12" r="2.8"/>')],
  ['wxy_ic_disc_32', 32, line('<circle cx="16" cy="16" r="12.5"/><circle cx="16" cy="16" r="7.5"/><circle cx="16" cy="16" r="2.6"/>')],
  ['wxy_ic_eject_24', 24, line('<path d="M12 4 5 13h14L12 4Z"/><path d="M5 17.5h14"/>')],

  ['wxy_ic_battery_16', 16, '<svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 24 24"><g stroke="#ffffff" stroke-width="2" fill="none" stroke-linejoin="round"><rect x="2.5" y="7" width="17" height="10" rx="2"/><path d="M21.5 10.5v3"/></g><rect x="5" y="10" width="9" height="4" fill="#ffffff"/></svg>'],
  ['wxy_ic_wifi_16', 16, '<svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 24 24"><g stroke="#ffffff" stroke-width="2" fill="none" stroke-linecap="round"><path d="M2.5 9a14 14 0 0 1 19 0"/><path d="M5.5 12.5a9.5 9.5 0 0 1 13 0"/><path d="M8.5 16a5 5 0 0 1 7 0"/></g><circle cx="12" cy="19.5" r="1.4" fill="#ffffff"/></svg>'],
  ['wxy_ic_signal_16', 16, '<svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 24 24"><g stroke="#ffffff" stroke-width="2.4" stroke-linecap="round"><path d="M4 20v-4M9.3 20v-8M14.7 20v-12M20 20V4"/></g></svg>'],

  ['wxy_ic_clock_24', 24, line('<circle cx="12" cy="12" r="9"/><path d="M12 6.5V12l4 2.5"/>')],
  ['wxy_ic_weather_24', 24, line('<circle cx="12" cy="12" r="4.5"/><path d="M12 2v2.5M12 19.5V22M2 12h2.5M19.5 12H22M4.9 4.9l1.8 1.8M17.3 17.3l1.8 1.8M19.1 4.9l-1.8 1.8M6.7 17.3l-1.8 1.8"/>')],
  ['wxy_ic_music_24', 24, '<svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 24 24"><g stroke="#ffffff" stroke-width="1.7" fill="none" stroke-linecap="round"><path d="M9 17.5V6l11-2v11.5"/></g><ellipse cx="6.2" cy="17.6" rx="2.8" ry="2.1" fill="#ffffff"/><ellipse cx="17.2" cy="15.6" rx="2.8" ry="2.1" fill="#ffffff"/></svg>'],
  ['wxy_ic_note_24', 24, line('<path d="M5 3h9l5 5v13H5V3Z"/><path d="M14 3v5h5"/><path d="M8 12h8M8 16h6"/>')],
  ['wxy_ic_heart_24', 24, solid('<path d="M12 20.6 3.7 12.3a4.9 4.9 0 0 1 6.9-6.9L12 7l1.4-1.6a4.9 4.9 0 0 1 6.9 6.9L12 20.6Z"/>')],

  ['wxy_ic_play_24', 24, solid('<path d="M7 4.2 20 12 7 19.8V4.2Z"/>')],
  ['wxy_ic_pause_24', 24, solid('<rect x="6" y="4.5" width="4" height="15" rx="1"/><rect x="14" y="4.5" width="4" height="15" rx="1"/>')],
  ['wxy_ic_prev_24', 24, solid('<path d="M18.5 5 8 12l10.5 7V5Z"/><rect x="4" y="5" width="2.6" height="14" rx="1"/>')],
  ['wxy_ic_next_24', 24, solid('<path d="M5.5 5 16 12 5.5 19V5Z"/><rect x="17.4" y="5" width="2.6" height="14" rx="1"/>')],
  ['wxy_ic_stop_24', 24, solid('<rect x="5" y="5" width="14" height="14" rx="2"/>')],
  ['wxy_ic_refresh_24', 24, line('<path d="M20 12a8 8 0 1 1-2.4-5.7"/><path d="M20.5 3.5v5.5H15"/>')],

  ['wxy_ic_check_16', 16, line('<path d="M4 12.5 9.5 18 20 6"/>')],
  ['wxy_ic_plus_24', 24, line('<path d="M12 4v16M4 12h16"/>')],
  ['wxy_ic_cloud_24', 24, line('<path d="M7 17.5a4.5 4.5 0 0 1-.5-9A6 6 0 0 1 18 9a4.3 4.3 0 0 1 0 8.5H7Z"/><path d="M12 10.5v6M9.5 14 12 16.5 14.5 14"/>')],

  ['wxy_ic_face_calm_32', 32, '<svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 24 24"><g stroke="#ffffff" stroke-width="1.7" fill="none" stroke-linecap="round"><circle cx="12" cy="12" r="9"/><path d="M9 15.2h6"/></g><circle cx="9" cy="9.8" r="1.15" fill="#ffffff"/><circle cx="15" cy="9.8" r="1.15" fill="#ffffff"/></svg>'],
  ['wxy_ic_face_happy_32', 32, '<svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 24 24"><g stroke="#ffffff" stroke-width="1.7" fill="none" stroke-linecap="round"><circle cx="12" cy="12" r="9"/><path d="M7.3 9.8q1.7-2.2 3.4 0M13.3 9.8q1.7-2.2 3.4 0"/><path d="M8 13.8a4.8 4.8 0 0 0 8 0"/></g></svg>'],
  ['wxy_ic_face_fire_32', 32, '<svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 24 24"><g stroke="#ffffff" stroke-width="1.7" fill="none" stroke-linecap="round"><circle cx="12" cy="12" r="9"/><path d="M6.8 8.8 10 10.2M17.2 8.8 14 10.2"/><path d="M7.8 14.2a5.2 5.2 0 0 0 8.4 0"/></g></svg>']
];

/* ------------------------------------------------------------------ */
/* 场景图：碟面 / 中心标签 / 小 CD                                      */
/* ------------------------------------------------------------------ */

function vinylField() {
  let grooves = '';
  for (let r = 88; r <= 274; r += 3.4) {
    const op = (r % 3 < 1.7) ? 0.55 : 0.35;
    grooves += `<circle cx="280" cy="280" r="${r.toFixed(1)}" fill="none" stroke="#2a2d35" stroke-width="0.7" opacity="${op}"/>`;
  }
  let marks = '';
  for (let i = 0; i < 12; i++) {
    const a = (i * 30) * Math.PI / 180;
    const x1 = 280 + 255 * Math.cos(a), y1 = 280 + 255 * Math.sin(a);
    const x2 = 280 + 268 * Math.cos(a), y2 = 280 + 268 * Math.sin(a);
    marks += `<line x1="${x1.toFixed(1)}" y1="${y1.toFixed(1)}" x2="${x2.toFixed(1)}" y2="${y2.toFixed(1)}" stroke="#3a3e48" stroke-width="1" opacity=".6"/>`;
  }
  return `<svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 560 560">
<defs>
 <radialGradient id="sheen" cx="34%" cy="26%" r="78%">
  <stop offset="0%" stop-color="#454954" stop-opacity=".42"/>
  <stop offset="48%" stop-color="#2b2e36" stop-opacity=".14"/>
  <stop offset="100%" stop-color="#0d0e11" stop-opacity="0"/>
 </radialGradient>
 <clipPath id="disc"><circle cx="280" cy="280" r="279"/></clipPath>
</defs>
<circle cx="280" cy="280" r="279" fill="#15161a"/>
<g clip-path="url(#disc)">${grooves}${marks}
 <circle cx="280" cy="280" r="279" fill="url(#sheen)"/>
 <path d="M280 1 A279 279 0 0 1 559 280 L280 280Z" fill="#ffffff" opacity=".018"/>
</g>
<circle cx="280" cy="280" r="279" fill="none" stroke="#4a4e59" stroke-width="1.2"/>
<circle cx="280" cy="280" r="82" fill="#15161a"/>
<circle cx="280" cy="280" r="82" fill="none" stroke="#333640" stroke-width="1"/>
</svg>`;
}

function vinylLabel() {
  return `<svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 156 156">
<defs><radialGradient id="paper" cx="38%" cy="30%" r="75%">
 <stop offset="0%" stop-color="#f2eee2"/><stop offset="100%" stop-color="#d9d3c3"/>
</radialGradient></defs>
<circle cx="78" cy="78" r="77" fill="url(#paper)"/>
<circle cx="78" cy="78" r="70" fill="none" stroke="#a9a292" stroke-width="1"/>
<circle cx="78" cy="78" r="66" fill="none" stroke="#bdb5a3" stroke-width=".6" opacity=".7"/>
<circle cx="78" cy="78" r="6.5" fill="#15161a"/>
<circle cx="78" cy="78" r="9" fill="none" stroke="#a9a292" stroke-width=".8"/>
<path d="M30 78h-8M134 78h-8" stroke="#a9a292" stroke-width="1"/>
</svg>`;
}

function miniCd() {
  let g = '';
  for (let r = 10; r <= 25; r += 2.2) {
    g += `<circle cx="28" cy="28" r="${r}" fill="none" stroke="#393d46" stroke-width=".5" opacity=".75"/>`;
  }
  return `<svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 56 56">
<circle cx="28" cy="28" r="27.5" fill="#17181c"/>${g}
<circle cx="28" cy="28" r="27.5" fill="none" stroke="#4a4e59" stroke-width="1"/>
<circle cx="28" cy="28" r="8.5" fill="#d9d3c3"/>
<circle cx="28" cy="28" r="2.2" fill="#17181c"/>
</svg>`;
}

const SCENES = [
  ['wxy_vinyl_field_560', 560, vinylField()],
  ['wxy_vinyl_label_156', 156, vinylLabel()],
  ['wxy_mini_cd_56', 56, miniCd()]
];

/* ------------------------------------------------------------------ */
/* 渲染：SVG -> canvas -> PNG dataURL -> Edge DOM -> Node 写文件         */
/* ------------------------------------------------------------------ */

const ALL = ICONS.concat(SCENES);

const html = `<!DOCTYPE html><html><head><meta charset="utf-8"></head><body>
<pre id="OUT">PENDING</pre>
<script>
const ITEMS = ${JSON.stringify(ALL)};
function drawOne(it) {
  return new Promise(function(resolve, reject) {
    var img = new Image();
    img.onload = function() {
      var c = document.createElement('canvas');
      c.width = it[1]; c.height = it[1];
      var x = c.getContext('2d');
      x.clearRect(0, 0, it[1], it[1]);
      x.drawImage(img, 0, 0, it[1], it[1]);
      resolve([it[0], c.toDataURL('image/png')]);
    };
    img.onerror = function() { reject(new Error('svg load failed: ' + it[0])); };
    img.src = 'data:image/svg+xml;charset=utf-8,' + encodeURIComponent(it[2]);
  });
}
function preview(results) {
  var map = {}; results.forEach(function(r) { map[r[0]] = r[1]; });
  var cols = 6, cw = 88, ch = 96;
  var rows = Math.ceil(${ICONS.length} / cols);
  var c = document.createElement('canvas');
  c.width = cols * cw; c.height = rows * ch;
  var x = c.getContext('2d');
  x.fillStyle = '#1b1c22'; x.fillRect(0, 0, c.width, c.height);
  x.textAlign = 'center'; x.font = '10px monospace'; x.fillStyle = '#9a9da8';
  ${ICONS.length > 0 ? '' : ''}
  var names = ${JSON.stringify(ICONS.map(i => i[0]))};
  var pending = names.map(function(n) {
    return new Promise(function(resolve) {
      var im = new Image();
      im.onload = function() {
        var i = names.indexOf(n), col = i % cols, row = Math.floor(i / cols);
        var cx = col * cw + cw / 2, cy = row * ch + 34;
        var s = im.width;
        x.drawImage(im, cx - s / 2, cy - s / 2);
        x.fillText(n.replace('wxy_ic_', '').replace(/\\d+$/, ''), cx, row * ch + 82);
        resolve();
      };
      im.onerror = resolve;
      im.src = map[n];
    });
  });
  return Promise.all(pending).then(function() { return ['_preview', c.toDataURL('image/png')]; });
}
Promise.all(ITEMS.map(drawOne)).then(function(r) {
  return preview(r).then(function(p) { r.push(p); return r; });
}).then(function(r) {
  document.getElementById('OUT').textContent = JSON.stringify(r);
  document.title = 'DONE';
}).catch(function(e) {
  document.getElementById('OUT').textContent = JSON.stringify([['ERROR', String(e)]]);
});
</script></body></html>`;

function findBrowser() {
  for (const p of EDGE_CANDIDATES) if (fs.existsSync(p)) return p;
  throw new Error('未找到 Edge/Chrome 浏览器，无法渲染 PNG');
}

function main() {
  fs.mkdirSync(TMP, { recursive: true });
  fs.mkdirSync(ICON_DIR, { recursive: true });
  fs.mkdirSync(UI_DIR, { recursive: true });

  const htmlPath = path.join(TMP, 'gen.html');
  fs.writeFileSync(htmlPath, html, 'utf8');

  const browser = findBrowser();
  const dom = cp.execFileSync(browser, [
    '--headless=new', '--disable-gpu', '--no-sandbox', '--hide-scrollbars',
    '--virtual-time-budget=20000', '--dump-dom', 'file:///' + htmlPath.replace(/\\/g, '/')
  ], { encoding: 'utf8', maxBuffer: 64 * 1024 * 1024 });

  const m = dom.match(/<pre id="OUT">([\s\S]*?)<\/pre>/);
  if (!m) throw new Error('未找到渲染结果节点');
  const raw = m[1].replace(/&quot;/g, '"').replace(/&amp;/g, '&')
                .replace(/&lt;/g, '<').replace(/&gt;/g, '>');
  const results = JSON.parse(raw);
  if (results.length === 1 && results[0][0] === 'ERROR') throw new Error(results[0][1]);

  const written = [];
  for (const [name, dataUrl] of results) {
    const isScene = name.startsWith('wxy_vinyl') || name.startsWith('wxy_mini');
    const dest = path.join(isScene ? UI_DIR : ICON_DIR,
      name === '_preview' ? '_preview.png' : name + '.png');
    fs.writeFileSync(dest, Buffer.from(dataUrl.split(',')[1], 'base64'));
    written.push(dest.replace(ROOT + path.sep, ''));
  }

  console.log('生成完成：' + written.length + ' 个文件');
  written.forEach(w => console.log('  ' + w));
  fs.rmSync(TMP, { recursive: true, force: true });
}

main();
