


class Stats:

    hot_data = []
    freq_data = []
    max_count_of_vars = 0
    def __init__(self, number_of_instructions) -> None:
        self.number_of_instructions = number_of_instructions

    def increment_instruction_count(self): 
        self.number_of_instructions += 1

    def give_num_of_instructions(self):
        return self.number_of_instructions

    def hot(self, input):   
        if not any(dictionary.get("key") == input["key"] for dictionary in self.hot_data):
            self.hot_data.append(input)
        else:
            for data in self.hot_data:
                if input["key"] == data["key"]:
                    data["count"] += 1
                    if int(input["order"]) < int(data["order"]):
                        data["order"] = input["order"]
    def give_hot(self):
        tmp = [{"key":None,"order":None,"count":0}]
        for data in self.hot_data:
            if tmp[0]["count"] < data["count"]:
                tmp[0]["count"] = data["count"]
                tmp[0]["key"] = data["key"]
                tmp[0]["order"] = data["order"]

        return tmp[0]["order"]

    def vars(self, variables):
        count = 0

        for key in variables:
            if variables[key][1] is not None:
                count += 1

        if count > self.max_count_of_vars:
            self.max_count_of_vars = count

    def give_max_vars(self):
        return self.max_count_of_vars

    def freq(self, input):
        if not any(dictionary.get("key") == input["key"] for dictionary in self.freq_data):
            self.freq_data.append(input)
        else:
            for data in self.freq_data:
                if input["key"] == data["key"]:
                    data["count"] += 1

    def give_freq(self):
        tmp = 0
        tmp_list = []
        for data in self.freq_data:
            if tmp < data["count"]:
                tmp = data["count"]
    
        for data in self.freq_data:
            if data["count"] == tmp:
                tmp_list.append(data["key"])

        return ",".join(tmp_list)