%global debug_package %{nil}

Name: ::package_name::-devel
Version: ::package_version::
Release: ::package_build_version::%{?dist}
Summary: ::package_description_short::
License: ::package_licence::
URL: ::package_url::
Source0: %{name}-%{version}.tar.gz
BuildArch: ::package_architecture_el::
Conflicts: lib45d

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

%files
/usr/include/45d/*
/usr/lib/*

%changelog
* Wed Sep 22 2021 Joshua Boudreau <jboudreau@45drives.com> 0.3.4-1
- Add sync and async options for send/receive data with sockets.
* Tue Sep 21 2021 Joshua Boudreau <jboudreau@45drives.com> 0.3.3-1
- Add socket method to shutdown socket.
* Tue Sep 21 2021 Joshua Boudreau <jboudreau@45drives.com> 0.3.2-1
- Add ability to send and receive std::vector<std::string> through sockets.
* Mon Sep 20 2021 Joshua Boudreau <jboudreau@45drives.com> 0.3.1-1
- Update documentation for sockets
* Mon Sep 20 2021 Joshua Boudreau <jboudreau@45drives.com> 0.3.0-1
- Add unix domain socket classes.
* Tue Sep 14 2021 Joshua Boudreau <jboudreau@45drives.com> 0.2.4-1
- Fixed copy constructor of Quota to not multiply by fraction while copying base
  Bytes.
* Tue Sep 14 2021 Joshua Boudreau <jboudreau@45drives.com> 0.2.3-2
- Fix el8-devel packaging.
* Tue Sep 14 2021 Joshua Boudreau <jboudreau@45drives.com> 0.2.3-1
- Add += and -= operators for ffd::Bytes.
- Remove dependency for boost::lexical_cast.
* Fri Sep 10 2021 Joshua Boudreau <jboudreau@45drives.com> 0.2.2-1
- Report which subsection errors happen in.
* Fri Sep 10 2021 Joshua Boudreau <jboudreau@45drives.com> 0.2.1-2
- Fix arch devel packaging.
* Fri Sep 10 2021 Joshua Boudreau <jboudreau@45drives.com> 0.2.1-1
- Allow ffd::Quota string constructor to parse Bytes and get ratio to max.
- Add an ffd::Quota constructor that takes two ffd::Bytes objects.
- Add ffd::Quota method to get maximum bytes.
- Add ffd::Bytes comparison operators and division resulting in double.
* Thu Sep 09 2021 Joshua Boudreau <jboudreau@45drives.com> 0.2.0-2
- Set up conflicts between regular and devel packages.
* Thu Sep 09 2021 Joshua Boudreau <jboudreau@45drives.com> 0.2.0-1
- Split out header file into multiple files and make library more generic.
* Thu Aug 05 2021 Joshua Boudreau <jboudreau@45drives.com> 0.1.0-2
- Fix devel packaging.
* Tue Aug 03 2021 Joshua Boudreau <jboudreau@45drives.com> 0.1.0-1
- First autopackaging build.