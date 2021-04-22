%if %{!?_desktopdir:1}
%define _desktopdir %{_datadir}/applications
%endif

Name:		grpn
Version:	1.5.2
Release:	1%{?dist}
Summary:	A RPN calcuator for real and complex numbers with a GTK / X11 GUI
Group:		Applications/System
License:	GPL
URL:		https://github.com/getreu/grpn

Source0:	%{name}-%{version}.tar.bz2

BuildRequires:	gtk2-devel, pango-devel

%description
GTK+ reverse polish notation calculator grpn is an RPN calculator
which uses the GTK+ widgets. It works with real and complex numbers,
has 4 different radix modes, supports general math functions along
with logarithmic and trigonometric functions.  Calculator stack is
only limited by free memory.

%prep
%setup -q -n %{name}

%build
%make_build %{?_smp_mflags} grpn

%install
rm -rf $RPM_BUILD_ROOT
%make_install -C src PREFIX=%{_prefix} MANDIR=%{_mandir}
%{__install} -m 0644 -D debian/grpn.desktop %{?buildroot}/%{_desktopdir}/grpn.desktop
%{__install} -m 0644 -D debian/grpn.xpm %{?buildroot}/%{_pixmapsdir}/grpn.xpm
%{__install} -m 0644 -D debian/grpn.png %{?buildroot}/%{_pixmapsdir}/grpn.png

%files
%doc LICENSE src/CHANGES README.md
%{_bindir}/%{name}
%{_mandir}/*/%{name}*
%{_desktopdir}/grpn.desktop
%{_pixmapsdir}/grpn.*

%changelog
* Sat Oct 26 2013 Nathaniel Clark <Nathaniel.Clark@misrule.us> - 1.3.4-1
- Initial Spec File
