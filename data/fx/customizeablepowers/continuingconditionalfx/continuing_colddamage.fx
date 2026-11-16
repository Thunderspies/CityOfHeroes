#############################################################
## Continuing_ColdDamage.fx
#############################################################

FXInfo

ClampMinScale 0.75 0.75 0.75
ClampMaxScale 1.25 1.25 1.25

#############################################################

Input
	Inpname	Hips
End

#########################################################

Condition
	On	Time
	Time	0

	Event
		Type	local
		At	chest
		bhvr	Behaviors\soundFade3.bhvr
		Sound Cold_loop 50.0 50.0 .42
		Flags	PowerLoopingSound
		Lifespan 110
	End

	Event
		Type	local
		At	chest
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part1	:IceBoltMistA.part
		Part2	:ChillTouchSnowA.part
	End
End

#########################################################

End