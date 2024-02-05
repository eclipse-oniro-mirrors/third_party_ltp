#!/bin/sh
# Copyright (c) 2018-2021 Petr Vorel <pvorel@suse.cz>
set -ex

yum="yum -y install --skip-broken"

$yum \
	asciidoc \
	autoconf \
	automake \
	make \
	clang \
	gcc \
	git \
	findutils \
	iproute \
	numactl-devel \
	libtirpc \
	libtirpc-devel \
	perl-JSON \
	perl-libwww-perl \
	pkg-config \
	redhat-lsb-core

# CentOS 8 fixes
$yum libmnl-devel || $yum libmnl
$yum rubygem-asciidoctor || true
