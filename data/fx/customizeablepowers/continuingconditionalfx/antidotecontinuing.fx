#############################################################
## AntidoteContinuing.fx
#############################################################

FxInfo

#########################################################

Input
	Inpname	Hips
End

#########################################################

Condition

	Event
		Type	local
		At	head

		Part	:BubblesTrailCont.part

		Part	:PoisonTrailCont.part
		Part	:PoisonTrailCont2.part
		Part	:DarkPoisonTrailCont.part
		Part	:DarkPoisonTrailCont2.part
		Part	:GlowstarsTrailCont2.part
		Sound Antidote1_loop 70 70 .33
		bhvr	behaviors/soundFade3.bhvr
		Flags	PowerLoopingSound
		Lifespan 200
	End

	Event
		Type	local
		At	chest

		Part	:ElipseCont.part


	End

End