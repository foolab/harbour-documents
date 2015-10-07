%define __provides_exclude_from ^%{_datadir}/.*$
%define __requires_exclude ^libjbig2dec.so.0|libjpeg.so.62|libopenjp2.so.7|libpoppler-qt5.so.1|libpoppler.so.54|libtiff.so.5|libfreetype.so.6|libGL.so.1|libX11.so.6|libGLEW.so.1.10|libavmedialo.so|libbasebmplo.so|libbasegfxlo.so|libcanvastoolslo.so|libchartcorelo.so|libclewlo.so|libclucene.so|libcomphelper.so|libconfigmgrlo.so|libcppcanvaslo.so|libdbalo.so|libdbtoolslo.so|libdbulo.so|libdeploymentmisclo.so|libdrawinglayerlo.so|libeditenglo.so|libetonyek-0.1.so.1|libexpwraplo.so|libexslt.so.0|libfilelo.so|libforlo.so|libforuilo.so|libfwelo.so|libfwilo.so|libfwklo.so|libfwllo.so|libGLEW.so.1.10|libhelplinkerlo.so|libi18nlangtag.so|libi18nutil.so|libicudata.so.54|libicui18n.so.54|libicuuc.so.54|liblcms2.so.2|liblnglo.so|liblocaledata_en.so|libmsfilterlo.so|libmwaw-0.3.so.3|libnspr4.so|libnss3.so|libnssutil3.so|libodfgen-0.1.so.1|libooxlo.so|libopencllo.so|libpackage2.so|libplc4.so|libplds4.so|libraptor2-lo.so.0|librasqal-lo.so.3|librdf-lo.so.0|libreglo.so|librevenge-0.0.so.0|librptlo.so|libsaxlo.so|libsblo.so|libsclo.so|libsdlo.so|libsfxlo.so|libsmime3.so|libsofficeapp.so|libsotlo.so|libsqlite3.so|libssl3.so|libstorelo.so|libsvllo.so|libsvtlo.so|libsvxcorelo.so|libsvxlo.so|libswlo.so|libtklo.so|libtllo.so|libucbhelper.so|libuno_cppuhelpergcc3.so.3|libuno_cppu.so.3|libunoidllo.so|libunopkgapp.so|libuno_purpenvhelpergcc3.so.3|libuno_salhelpergcc3.so.3|libuno_sal.so.3|libutllo.so|libvbahelperlo.so|libvcllo.so|libwpd-0.10.so.10|libwpg-0.3.so.3|libwps-0.4.so.4|libwriterperfectlo.so|libxmlreaderlo.so|libxmlscriptlo.so|libxmlsecurity.so|libxolo.so|libxslt.so.1|libxml2.so.2|libxml2.so.2(LIBXML2_GLOBAL_VARIABLES)$

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
BuildRequires:  libreoffice
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
rm -rf external/mupdf/thirdparty

%build
cmake .
make %{?jobs:-j%jobs}

%install
rm -rf %{buildroot}
%make_install

mkdir -p %{buildroot}/%{_datadir}/harbour-documents/lib/
cp -a deps/usr/lib/*.so.* %{buildroot}/%{_datadir}/harbour-documents/lib/
cp -a fakelibs/libGL.so.1 fakelibs/libX11.so.6 %{buildroot}/%{_datadir}/harbour-documents/lib/
mkdir -p %{buildroot}/%{_datadir}/harbour-documents/lib/libreoffice/
cp -a /usr/lib/libreoffice/instdir %{buildroot}/%{_datadir}/harbour-documents/lib/libreoffice/

# We don't want those
rm -rf %{buildroot}/%{_datadir}/harbour-documents/lib/libreoffice/instdir/program/regmerge
rm -rf %{buildroot}/%{_datadir}/harbour-documents/lib/libreoffice/instdir/program/oosplash
rm -rf %{buildroot}/%{_datadir}/harbour-documents/lib/libreoffice/instdir/program/uri-encode
rm -rf %{buildroot}/%{_datadir}/harbour-documents/lib/libreoffice/instdir/program/soffice.bin
rm -rf %{buildroot}/%{_datadir}/harbour-documents/lib/libreoffice/instdir/program/regview
rm -rf %{buildroot}/%{_datadir}/harbour-documents/lib/libreoffice/instdir/program/uno.bin
rm -rf %{buildroot}/%{_datadir}/harbour-documents/lib/libreoffice/instdir/program/gengal.bin
rm -rf %{buildroot}/%{_datadir}/harbour-documents/lib/libreoffice/instdir/program/unopkg.bin
rm -rf %{buildroot}/%{_datadir}/harbour-documents/lib/libreoffice/instdir/sdk/bin/idlc
rm -rf %{buildroot}/%{_datadir}/harbour-documents/lib/libreoffice/instdir/sdk/bin/javamaker
rm -rf %{buildroot}/%{_datadir}/harbour-documents/lib/libreoffice/instdir/sdk/bin/cppumaker
rm -rf %{buildroot}/%{_datadir}/harbour-documents/lib/libreoffice/instdir/sdk/bin/ucpp
rm -rf %{buildroot}/%{_datadir}/harbour-documents/lib/libreoffice/instdir/CREDITS.fodt
rm -rf %{buildroot}/%{_datadir}/harbour-documents/lib/libreoffice/instdir/LICENSE*
rm -rf %{buildroot}/%{_datadir}/harbour-documents/lib/libreoffice/instdir/NOTICE
rm -rf %{buildroot}/%{_datadir}/harbour-documents/lib/libreoffice/instdir/help
rm -rf %{buildroot}/%{_datadir}/harbour-documents/lib/libreoffice/instdir/presets
rm -rf %{buildroot}/%{_datadir}/harbour-documents/lib/libreoffice/instdir/sdk
rm -rf %{buildroot}/%{_datadir}/harbour-documents/lib/libreoffice/instdir/program/gengal
rm -rf %{buildroot}/%{_datadir}/harbour-documents/lib/libreoffice/instdir/program/sbase
rm -rf %{buildroot}/%{_datadir}/harbour-documents/lib/libreoffice/instdir/program/scalc
rm -rf %{buildroot}/%{_datadir}/harbour-documents/lib/libreoffice/instdir/program/sdraw
rm -rf %{buildroot}/%{_datadir}/harbour-documents/lib/libreoffice/instdir/program/senddoc
rm -rf %{buildroot}/%{_datadir}/harbour-documents/lib/libreoffice/instdir/program/simpress
rm -rf %{buildroot}/%{_datadir}/harbour-documents/lib/libreoffice/instdir/program/smath
rm -rf %{buildroot}/%{_datadir}/harbour-documents/lib/libreoffice/instdir/program/soffice
rm -rf %{buildroot}/%{_datadir}/harbour-documents/lib/libreoffice/instdir/program/swriter
rm -rf %{buildroot}/%{_datadir}/harbour-documents/lib/libreoffice/instdir/program/uno
rm -rf %{buildroot}/%{_datadir}/harbour-documents/lib/libreoffice/instdir/program/unoinfo
rm -rf %{buildroot}/%{_datadir}/harbour-documents/lib/libreoffice/instdir/program/unopkg

desktop-file-install --delete-original                   \
  --dir %{buildroot}%{_datadir}/applications             \
   %{buildroot}%{_datadir}/applications/*.desktop

%files
%defattr(-,root,root,-)
%{_bindir}/harbour-documents
%{_datadir}/applications/harbour-documents.desktop
%{_datadir}/icons/hicolor/86x86/apps/harbour-documents.png
%{_datadir}/harbour-documents/lib/*.so.*
%{_datadir}/harbour-documents/lib/libreoffice/*
