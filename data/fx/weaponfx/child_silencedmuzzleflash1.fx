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
		BHVR	WEAPONs\MussleFlashOffset.bhvr
		Part1	WEAPONFX\SilencedMachineGunFlash.part
		Part2	WEAPONFX\SilencedMachineGunGlow.part
		Part2	WEAPONFX\SilencedMachineGunFlashcore.part
		Lifespan 2
	End
End

#############################################################

End