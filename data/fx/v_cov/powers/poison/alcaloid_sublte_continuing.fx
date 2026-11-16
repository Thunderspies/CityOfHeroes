#############################################################
## Alcaloid_Sublte_Continuing.fx
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
		At	Chest

		Part	:QuillTrailBlackContz.part
		Part	:QuillTrailLightContz.part
		Part	:QuillBubblesTrailContz.part
		Part	:QuillTrailLightCont2z.part
		Part	:QuillTrailBlackCont2z.part
		Part	:QuillTrailElipseContz.part
		Part	:QuillBubblesTrailCont2z.part
		Sound Alkaloid1_loop 60 60 .66
		bhvr	behaviors/soundFade3.bhvr
		Flags	PowerLoopingSound
		Lifespan 200


	End

End

End