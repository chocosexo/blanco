import mqtt from "mqtt";
import { config } from "@/config";
const mqttClientSingleton = () => {
  return mqtt.connect(config.MQTT_URL);
};

declare const globalThis: {
  mqttGlobal: ReturnType<typeof mqttClientSingleton>;
} & typeof global;

const socketMqtt = globalThis.mqttGlobal ?? mqttClientSingleton();

export default socketMqtt;

if (process.env.NODE_ENV !== "production") globalThis.mqttGlobal = socketMqtt;
