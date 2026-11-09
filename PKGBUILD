# Maintainer: Monasm <monasm@gchan.moe> 

pkgname=momoisay-bin
pkgver=1.2.0
pkgrel=1
pkgdesc="momoisay is a CLI Program like cowsay, but instead of a talking cow, it's Saiba Momoi from Blue Archive!!! Make Momoi say anything you want with various Momoi ASCII-art for you to choose, perfect for adding some Blue Archive energy to your PC."
arch=('x86_64')
url="https://github.com/Mon4sm/momoisay"
license=('GPL-3.0')
depends=('ncurses')
makedepends=('git')
provides=('momoisay-bin')
conflicts=('momoisay-bin')
source=("git+https://github.com/Mon4sm/momoisay.git")
sha256sums=('SKIP')

package() {
	install -D "$srcdir/momoisay/bin/linux/momoisay" \
		"$pkgdir/usr/local/bin/momoisay"
}
