%define __provides_exclude_from ^%{_datadir}/.*$
%define __requires_exclude ^libjbig2dec.so.0|libjpeg.so.62|libopenjp2.so.7|libpoppler-qt5.so.1|libpoppler.so.54|libtiff.so.5|libfreetype.so.6$

Name:       harbour-documents
Summary:    Document viewer application for Sailfish OS
Version:    0.0.0
Release:    1
Group:      Applications
License:    GPLv3
URL:        https://github.com/foolab/harbour-documents
Source0:    %{name}-%{version}.tar.bz2
BuildRequires:  pkgconfig(Qt5Qml)
BuildRequires:  pkgconfig(Qt5Quick)
BuildRequires:  pkgconfig(Qt5Xml)
BuildRequires:  pkgconfig(Qt5Test)
BuildRequires:  pkgconfig(libpng)
BuildRequires:  pkgconfig(qdeclarative5-boostable)
BuildRequires:  desktop-file-utils
BuildRequires:  cmake
BuildRequires:  gettext
%ifarch i486
BuildRequires:  yasm
%endif
Requires:  sailfishsilica-qt5
Requires:  mapplauncherd-booster-silica-qt5
Requires:  nemo-qml-plugin-configuration-qt5

%description
 A document viewer for Sailfish OS

%prep
%setup -q

%build
cmake .
make %{?jobs:-j%jobs}

%install
rm -rf %{buildroot}
%make_install
mkdir -p %{buildroot}/%{_datadir}/harbour-documents/lib/
cp -a deps/usr/lib/*.so.* %{buildroot}/%{_datadir}/harbour-documents/lib/

desktop-file-install --delete-original                   \
  --dir %{buildroot}%{_datadir}/applications             \
   %{buildroot}%{_datadir}/applications/*.desktop

%files
%defattr(-,root,root,-)
%{_bindir}/harbour-documents
%{_datadir}/applications/harbour-documents.desktop
%{_datadir}/icons/hicolor/86x86/apps/harbour-documents.png
%{_datadir}/harbour-documents/lib/*.so.*
