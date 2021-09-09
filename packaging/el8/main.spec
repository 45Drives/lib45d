%global debug_package %{nil}

Name: ::package_name::
Version: ::package_version::
Release: ::package_build_version::%{?dist}
Summary: ::package_description_short::
License: ::package_licence::
URL: ::package_url::
Source0: %{name}-%{version}.tar.gz
BuildArch: ::package_architecture_el::
Conflicts: lib45d-devel

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

%files
/usr/lib/*

%changelog
* Thu Sep 09 2021 Joshua Boudreau <jboudreau@45drives.com> 0.2.0-1
- Split out header file into multiple files and make library more generic.
* Thu Aug 05 2021 Joshua Boudreau <jboudreau@45drives.com> 0.1.0-2
- Fix devel packaging.
* Tue Aug 03 2021 Joshua Boudreau <jboudreau@45drives.com> 0.1.0-1
- First autopackaging build.