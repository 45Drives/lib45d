%global debug_package %{nil}

Name: ::package_name::
Version: ::package_version::
Release: ::package_build_version::%{?dist}
Summary: ::package_description_short::
License: ::package_licence::
URL: ::package_url::
Source0: %{name}-%{version}.tar.gz
BuildArch: ::package_architecture_el::

BuildRoot: %{_tmppath}/%{name}-%{version}-%{release}-root

%description
::package_title::
::package_description_long::

%prep
%setup -q

%build
make

%install
make DESTDIR=%{buildroot} install

%post
ldconfig -v

%files
/opt/45drives/*
/etc/ld.so.conf.d/45drives.conf

%changelog
* Thu Aug 05 2021 Joshua Boudreau <jboudreau@45drives.com> 0.1.0-2
- Fix devel packaging.
* Thu Aug 05 2021 Joshua Boudreau <jboudreau@45drives.com> 0.1.0-2
- Fix devel packaging.
* Tue Aug 03 2021 Joshua Boudreau <jboudreau@45drives.com> 0.1.0-1
- First autopackaging build.