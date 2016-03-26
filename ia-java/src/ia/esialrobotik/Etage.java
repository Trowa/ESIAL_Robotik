package ia.esialrobotik;

import org.mbed.RPC.MbedRPC;
import org.mbed.RPC.RPCVariable;

/**
 * Created by mickael on 14/05/15.
 */
public class Etage {
	private RPCVariable<Float> setPoint;
	private RPCVariable<Float> position;
	private RPCVariable<Integer> enable;
	private RPCVariable<Integer> endstop;
	private RPCVariable<Float> speed;
	private RPCVariable<Float> output;
	private long timeout;

	public Etage(MbedRPC rpc, long timeout) {
		this.setPoint = new RPCVariable<>(rpc, "SetPoint");
		this.position = new RPCVariable<>(rpc, "Position");
		this.enable = new RPCVariable<>(rpc, "Enable");
		this.endstop = new RPCVariable<>(rpc, "Endstop");
		this.output = new RPCVariable<>(rpc, "Output");
		this.timeout = timeout;

		this.setEnabled(false);
	}

	public void takeOrigin() {
		this.setEnabled(false);

		while (endstop.read_int() == 1) {
			this.output.write(-0.7f);
		}

		this.output.write(0f);
		this.setPosition(0.04f, true);
		this.setEnabled(true);
	}

	public float getPosition() {
		return this.position.read_float();
	}

	public void setPosition(float pos, boolean blocking) {
		this.setPoint.write(pos);

		if (blocking) {
			long time = System.currentTimeMillis();

			while (this.output.read_float() != 0f) {
				if (timeout > 0 && (System.currentTimeMillis() - time) > timeout) {
					return;
				}
			}
		}
	}

	public void setEnabled(boolean val) {
		this.enable.write(val ? 1 : 0);
	}

	public void setOutput(float out) {
		this.output.write(out);
	}
}
