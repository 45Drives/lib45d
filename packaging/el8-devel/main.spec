%global debug_package %{nil}

Name: ::package_name::-devel
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
make dev

%install
make DESTDIR=%{buildroot} DEVEL=1 install

%post
ldconfig -v

%files
/opt/45drives/*
/etc/ld.so.conf.d/45drives.conf

%changelog
* Tue Aug 03 2021 Joshua Boudreau <jboudreau@45drives.com> 0.1.0-1
- First autopackaging buid.