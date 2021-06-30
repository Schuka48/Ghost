import socket
import json
import threading
import subprocess
import sys

HEADER = 64
FORMAT = 'utf-8'
DISCONNECT_MESSAGE = "!DISCONNECT"

server_port = 8888
# server_ip = socket.gethostbyname(socket.gethostname())
# server_ip = '127.0.0.1'

server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

while True:
    try:
        server_ip = input('Enter ip address of the server -> ')
        server.bind((server_ip, server_port))
        break
    except OSError as e:
        print(e)
        print("Try again ...")

clients_list = []
clients_now = []

closed = False


def error_check(answer):
    for ans in answer:
        if ans.find("KeyError:", 0) != -1:
            answer.append("This config has some Errors")
            return True
    return False


def create_vpc(jObj):
    answer = "success"
    vpc_params = jObj["params"]
    # vpc_id = int(vpc_params["id"])
    vpc_name = vpc_params["name"]
    try:
        script = subprocess.Popen(['./create_netns.sh', vpc_name], stdout=subprocess.PIPE)
        answer = script.communicate()[0].decode(FORMAT)
    except FileNotFoundError:
        answer = "Error: File_Not_Found"
    print(answer)

    return answer


def show_netns(jObj):
    answer = "success"
    try:
        script = subprocess.Popen(['./show_netns.sh'], stdout=subprocess.PIPE)
        answer = script.communicate()[0].decode(FORMAT)
    except FileNotFoundError:
        answer = "Error:File_Not_Found"
    print(answer)
    return answer


def create_bridge(jObj):
    answer = "success"
    bridge_params = jObj["params"]
    # print(bridge_params)
    # bridge_id = int(bridge_params["id"])
    bridge_name = bridge_params["name"]
    br_ip = bridge_params["br_ip"]
    try:
        script = subprocess.Popen(['./create_br.sh', bridge_name], stdout=subprocess.PIPE)
        answer = script.communicate()[0].decode(FORMAT)
    except FileNotFoundError:
        answer = "Error: File_Not_Found"
    print(answer)
    return answer


def create_link(jObj):
    answer = "success"
    int_name = []
    for interface in jObj["interfaces"]:
        name = interface["int_name"]
        int_name.append(name)
    try:
        script = subprocess.Popen(['./create_int.sh', int_name[0], int_name[1]], stdout=subprocess.PIPE)
        answer = script.communicate()[0].decode(FORMAT)
    except FileNotFoundError:
        answer = "Error: File_Not_Found"
    print(answer)
    return answer


def connect_to_bridge(jObj):
    answer = "success"
    interface = jObj["interface"]
    int_name = interface["int_name"]
    bridge_name = interface["bridge_name"]
    try:
        script = subprocess.Popen(['./term_int_ph.sh', int_name, bridge_name], stdout=subprocess.PIPE)
        answer = script.communicate()[0].decode(FORMAT)
    except FileNotFoundError:
        answer = "Error: File_Not_Found"
    print(answer)
    return answer


def connect_to_vpc(jObj):
    answer = "success"

    interface = jObj["interface"]
    ns_name = interface["vpc_name"]
    int_name = interface["int_name"]
    try:
        script = subprocess.Popen(['./term_int.sh', ns_name, int_name], stdout=subprocess.PIPE)
        answer = script.communicate()[0].decode(FORMAT)
    except FileNotFoundError:
        answer = "Error: File_Not_Found"
    print(answer)
    return answer


def ip_int_assign(jObj):
    answer = "success"
    interface = jObj["interface"]
    int_name = interface["int_name"]
    ns_name = interface["vpc_name"]
    int_address = interface["address"]
    try:
        script = subprocess.Popen(['./term_ip_vpc.sh', ns_name, int_address, int_name], stdout=subprocess.PIPE)
        answer = script.communicate()[0].decode(FORMAT)
    except FileNotFoundError:
        answer = "Error: File_Not_Found"
    print(answer)
    return answer


def ip_br_assign(jObj):
    answer = "success"
    params = jObj["params"]
    br_ip = params["br_ip"]
    br_name = params["name"]
    try:
        script = subprocess.Popen(['./term_ip_br.sh', br_ip, br_name], stdout=subprocess.PIPE)
        answer = script.communicate()[0].decode(FORMAT)
    except FileNotFoundError:
        answer = "Error: File_Not_Found"
    print(answer)
    return answer


def create_route(jObj):
    answer = "success"
    route = jObj["route_params"]

    gw_ip = route["gateway_ip"]
    net = route["network"]

    stack = route["container"]
    type_stack = stack["type"]
    if type_stack == "vpc":
        ns_name = stack["name"]
        try:
            script = subprocess.Popen(['./route.sh', ns_name, net, gw_ip], stdout=subprocess.PIPE)
            answer = script.communicate()[0].decode(FORMAT)
        except FileNotFoundError:
            answer = "Error: File_Not_Found"
    elif type_stack == "phis":
        try:
            script = subprocess.Popen(['./route_ph.sh', net, gw_ip], stdout=subprocess.PIPE)
            answer = script.communicate()[0].decode(FORMAT)
        except FileNotFoundError:
            answer = "Error: File_Not_Found"
    print(answer)
    return answer


def create_tc_rule(jObj):
    answer = "success"
    tc = jObj["params"]
    int_name = tc["interface_name"]
    jitter = tc["jitter"]
    loss = tc["loss"]
    package_order = tc["package_order"]
    noise = tc["noise"]

    try:
        script = subprocess.Popen(['./traffic_control', int_name, str(jitter[0]), str(jitter[1]), str(loss[0]), str(loss[1]),
                                   str(package_order[0]), str(package_order[1]), str(noise)], stdout=subprocess.PIPE)
        answer = script.communicate()[0].decode(FORMAT)
    except FileNotFoundError:
        answer = "Error: File_Not_Found"
    print(answer)
    return answer


def assign_tc_rules(jObj):
    answer = "success"
    tc = jObj["params"]
    int_name = tc["interface_name"]
    jitter = tc["jitter"]
    loss = tc["loss"]
    package_order = tc["package_order"]
    noise = tc["noise"]

    try:
        script = subprocess.Popen(['./traffic_control_as', int_name, str(jitter[0]), str(jitter[1]), str(loss[0]), str(loss[1]),
                                   str(package_order[0]), str(package_order[1]), str(noise)], stdout=subprocess.PIPE)
        answer = script.communicate()[0].decode(FORMAT)
    except FileNotFoundError:
        answer = "Error: File_Not_Found"
    print(answer)
    return answer


def create_network(jObj):
    answer = []
    time = jObj["time"]
    date = jObj["date"]
    description = jObj["description"]
    network = jObj["network"]
    bridge = network["network_bridge"]
    bridge_name = ""
    bridge_ip = ""
    vpcs = []
    ints = []

    print(f"[TRYING]Starting loading config: {description}, created {date} in {time} ... ")

    try:
        bridge_name = bridge["name"]
        bridge_ip = bridge["ip_addr"]
        ans = create_bridge(jObj={"params": {"name": bridge_name, "br_ip": bridge_ip}})
        answer.append(ans)
        if len(bridge_ip) != 0:
            ans = ip_br_assign(jObj={"params": {"name": bridge_name, "br_ip": bridge_ip}})
            answer.append(ans)
    except json.decoder.JSONDecodeError as err:
        answer.append(err)
    try:
        net_vpcs = network["network_vpcs"]
        for vpc in net_vpcs:
            vpc_name = vpc["name"]
            vpcs.append(vpc_name)
            ans = create_vpc(jObj={"params": {"name": vpc_name}})
            answer.append(ans)
    except json.decoder.JSONDecodeError as err:
        answer.append(err)

    try:
        net_interfaces = network["network_interfaces"]
        i = 0
        for interface in net_interfaces:
            int_name = interface["name"]
            ints.append(int_name)
            if i % 2 != 0:
                # запуск скрипта на создание пары интерфейсов
                jInts = []
                jInts.append({"int_name": ints[i - 1]})
                jInts.append({"int_name": ints[i]})
                ans = create_link(jObj={"interfaces": jInts})
                answer.append(ans)
            i += 1

            #  Пробегаемся по интерфейсам и присоединяем их соответствующим стекам + если необходимо,
            #  назначаем адреса интерфейсам

        for interface in net_interfaces:
            int_br_name = interface["br_name"]
            int_pc_name = interface["pc_name"]
            int_vpc_name = interface["vpc_name"]
            if len(int_br_name):
                ans = connect_to_bridge(jObj={"interface": {"int_name": interface["name"], "bridge_name": int_br_name}})
                answer.append(ans)
                continue
            elif len(int_vpc_name):
                ans = connect_to_vpc(jObj={"interface": {"int_name": interface["name"], "vpc_name": int_vpc_name}})
                answer.append(ans)
            if len(interface["ip_addr"]):
                ans = ip_int_assign(jObj={"interface": {"int_name": interface["name"], "address": interface["ip_addr"],
                                                        "vpc_name": interface["vpc_name"]}})
                answer.append(ans)

    except json.decoder.JSONDecodeError as err:
        answer.append(err)

        #  Создаем маршруты сети

    try:
        routes = network["network_routes"]
        for route in routes:
            type_route = ""
            ns_name = ""
            if route["pc_route"]:
                type_route = "phis"
            elif route["vpc_route"]:
                type_route = "vpc"
                ns_name = route["vpc_param"]["name"]
            ans = create_route(jObj={"route_params": {
                "gateway_ip": route["gateway_ip"],
                "network": route["net_destination"],
                "container": {
                    "type": type_route,
                    "name": ns_name
                }}})
            answer.append(ans)
    except json.decoder.JSONDecodeError as err:
        answer.append(err)

    try:
        tcs = network["network_tc"]
        for tc in tcs:
            jObj = {"params": tc}
            ans = create_tc_rule(jObj=jObj)
            answer.append(ans)
    except json.decoder.JSONDecodeError as err:
        answer.append(err)

    #     Проверка answer на наличие ошибок загрузки конфига

    error_check(answer=answer)

    return answer


def handle_client(client_sock, client_addr):
    try:
        print(f"[NEW CONNECTION] client from {client_addr} connected...")
        connected = True
        answer = "Error JSON Format!"
        while connected:
            msg = client_sock.recv(65535).decode(FORMAT)
            msg_json = json.loads(msg)
            # print(msg_json)
            command = None
            stack = None
            try:
                command = msg_json["command"]
            except KeyError:
                command = "error_json"
            try:
                stack = msg_json["stack"]
            except KeyError:
                stack = "error_stack"
            # KeyError
            if command == "create":
                if stack == "vpc":
                    answer = create_vpc(jObj=msg_json)
                elif stack == "bridge":
                    answer = create_bridge(jObj=msg_json)
                elif stack == "link":
                    answer = create_link(jObj=msg_json)
                elif stack == "route":
                    answer = create_route(jObj=msg_json)
                elif stack == "network":
                    answer = create_network(jObj=msg_json)
                elif stack == "tc":
                    answer = create_tc_rule(jObj=msg_json)
            elif command == "show":
                if stack == "vpc":
                    answer = show_netns(jObj=msg_json)
            elif command == "disconnect":
                js_disc = {}
                js_disc["command"] = "disconnect"
                js_disc["answer"] = f"disconnect from {server_ip} : {server_port}"
                js_disc = json.dumps(js_disc).encode(FORMAT)
                client_sock.send(js_disc)
                raise ConnectionResetError
            elif command == "connect":
                if stack == "bridge":
                    answer = connect_to_bridge(jObj=msg_json)
                else:
                    answer = connect_to_vpc(jObj=msg_json)
            elif command == "assign":
                if stack == "interface":
                    answer = ip_int_assign(jObj=msg_json)
                elif stack == "bridge":
                    answer = ip_br_assign(jObj=msg_json)
                elif stack == "tc":
                    answer = assign_tc_rules(jObj=msg_json)
            answer_json = {}
            answer_json["command"] = command
            answer_json["stack"] = stack
            answer_json["answer"] = answer
            answer_msg = json.dumps(answer_json)

            # В конце обработки приходящего запроса отправляем обратно ответ
            client_sock.send(answer_msg.encode(FORMAT))
    except ConnectionResetError:
        ind = clients_now.index(client_addr)
        clients_now.remove(client_addr)
        print(f'[CLOSE] client from {client_addr} disconnected ...')
        client_sock.close()
        return
    except json.decoder.JSONDecodeError as err:
        print(err)
        return


def start_server():
    server.listen(1)

    while not closed:
        conn, addr = server.accept()
        clients_list.append(addr)
        clients_now.append(addr)
        thread = threading.Thread(target=handle_client, args=(conn, addr))
        thread.start()

        print(f"[ACTIVE CONNECTIONS] {threading.activeCount() - 1}")
        print(f"Now connected {clients_now}")
    server.close()


print(f"[STARTING]Starting server on {server_ip} and port № {server_port} ...")

start_server()
