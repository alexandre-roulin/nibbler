#!/bin/bash
export id=()
index=0
pid=()

pidOfScript="$(echo ${$})"

function trap_with_arg() {
    func="$1" ; shift
    for sig ; do
        trap "${func} ${sig}" "$sig"
    done
}
function func_trap() {
    echo Trapped: $1
    index=0
    while test ${index} != ${numOfClient}
        do
        kill -n 30 ${pid[$index]}
        echo "kill -n 30 ${pid[$index]}"
        index=$(($index + 1))
    done
}
function in_id {
  for e in ${id[*]}
  do
    if [[ "$e" == "$1" ]]
    then
        echo "$e == $1"
      return 1
    fi
  done
  return 0
}

#trap_with_arg func_trap SIGUSR1

echo "pidOfScript : ${pidOfScript}"

while read ligne ; do
    checkId="$(echo ${ligne} | cut -d' ' -f1)"
    if in_id ${checkId}
    then
        id+=("$(echo ${checkId})")
    fi
done < $1

for element in "${id[@]}"
do
    echo "ID [$element]"sleep
done

numOfClient=${#id[@]}

while test ${index} != ${numOfClient}
    do
    ./nibbler -t --id ${id[$index]} --fileInput ${1} --pidTestProcess ${pidOfScript} >> /tmp/log_test.out & pid[$index]=$! > /tmp/log_pid.out
    echo ${id[$index]}
    index=$(($index + 1))
done

sleep 2

index=0
while test ${index} != ${numOfClient}
    do
    kill -n 30 ${pid[$index]}
    echo "kill -n 30 ${pid[$index]}"
    index=$(($index + 1))
done

index=0
while test ${index} != ${numOfClient}
    do
    echo "${pid[$index]}"
    index=$(($index + 1))
done

for p in ${pid[*]}; do
    while kill -0 ${p}; do
        sleep 0.01
    done
done

exit 0