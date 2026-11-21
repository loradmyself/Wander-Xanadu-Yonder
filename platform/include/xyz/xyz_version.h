/*
 * SPDX-License-Identifier: Apache-2.0
 *
 * WXY 平台版本标识。
 * 这里只放"编译期常量"级信息，不引入任何依赖，应用与平台层都可以安全包含。
 */

#ifndef XYZ_VERSION_H
#define XYZ_VERSION_H

#define XYZ_VERSION_MAJOR   0
#define XYZ_VERSION_MINOR   1
#define XYZ_VERSION_PATCH   0

#define XYZ_STR_(x)         #x
#define XYZ_STR(x)          XYZ_STR_(x)
#define XYZ_VERSION_STRING  XYZ_STR(XYZ_VERSION_MAJOR) "." \
                            XYZ_STR(XYZ_VERSION_MINOR) "." \
                            XYZ_STR(XYZ_VERSION_PATCH)

/* 动态应用包（.xzp）与工具契约的 ABI 版本。改动对外 ABI 头时必须加一。 */
#define XYZ_ABI_VERSION     1

/* 产品名，仅用于启动横幅与日志，不参与逻辑。 */
#define XYZ_PRODUCT_NAME    "Wander-Xanadu-Yonder"

#endif /* XYZ_VERSION_H */
