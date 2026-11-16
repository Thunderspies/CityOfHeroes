#############################################################
## Child Effect - Muzzle Flash Cycle (Rapid Fire)
#############################################################

FxInfo

Lifespan 180

#############################################################

Condition
	On 	Cycle
	Time 	3

	Event
		Type	Local
		At	Origin
		Bhvr	Behaviors\MachineGunBust.bhvr
		Part1	WEAPONFX\MachineGunFlash.part
		Part2	WEAPONFX\MachineGunGlow.part
		Geom	FX_MachineGunBurst
		Lifespan 2
	End
End

#############################################################

End