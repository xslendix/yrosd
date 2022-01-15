# Drip Server

This project implements a protocol for robot <-> client(s) communication.

## Getting started

```
$ git clone --depth 1 --recurse-submodules --shallow-submodules https://github.com/xslendix/dripd
$ cd dripd
$ ./dripd.sh build # You can also use `all` to build AND deploy.
```

## Deploying

You can either do this manually by copying dripd.conf and build/dripd or by using the recommended way
which is:
 - Copy `settings.example.sh` into `settings.sh`
 - Modify the copy
 - Run `./dripd.sh deploy`

By default, the server runs on port _9080_, _9081_ and _9082_. This can be changed in the configuration file as well.

## License

This project is licensed under the GNU AGPLv3 License. See the [LICENSE](LICENSE.txt) file for the full license text.
