# 
# qcma spec file
# 

%define _version 0.3.7

%if "%{_version}" == "testing" || "%{_version}" == "master"
%define _verprefix
%else
%define _verprefix v
%endif

Name:           qcma
Summary:        PSVita Content Manager Assistant
License:        GPL-3.0
Release:        1
Version:        %{_version}
URL:            https://github.com/codestation/qcma
Source:         https://github.com/codestation/qcma/archive/%{_verprefix}%{_version}/qcma-%{_version}.tar.gz
Group:          Productivity/File utilities
Requires:       ffmpeg
Requires:       qt5-qtbase
Requires:       qt5-qtimageformats
Requires:       libvitamtp4 >= 2.5.5
BuildRequires:  pkgconfig
BuildRequires:  ffmpeg-devel
BuildRequires:  libvitamtp-devel
BuildRequires:  qt5-qttools-devel
BuildRequires:  qt5-qtbase-devel

%description
QCMA is an cross-platform application to provide a Open Source implementation
of the original Content Manager Assistant that comes with the PS Vita. QCMA
is meant to be compatible with Linux, Windows and MAC OS X.

%setup -n %{name}-%{version} -DT

%build
lrelease-qt5 resources/translations/*.ts
qmake-qt5 PREFIX=/usr qcma.pro CONFIG+=QT5_SUFFIX
make %{?_smp_mflags}

%install
make install INSTALL_ROOT=%{buildroot}

%files
%defattr(-,root,root)
%{_bindir}/qcma
%{_bindir}/qcma_cli
%{_prefix}/share/applications/qcma/qcma.desktop
%{_prefix}/share/icons/hicolor/64x64/apps/qcma.png

%changelog
