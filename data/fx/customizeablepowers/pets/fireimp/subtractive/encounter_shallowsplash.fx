#############################################################
## Encounter_ShallowSplash.fx
#############################################################

FxInfo
Lifespan 10

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		EName 	Prime8
		Type	Start
		At	mystic

		Part	:Lava_dropsHit.part
		Part	:Lava_dropsLargerHit.part
  		Part    :Lava_GlobsLargerHit.part
		Part    :Lava_dropsBlackHit.part
		Part    :Lava_GlobBlackHit.part
		Part    :Lava_dropsBlackBigHit.part

	End


End

Condition
	On 	Time
	Time 	3

	Event
		EName 	Prime8
		Type	Start
		At	mystic

		Part	:Lava_dropsHit.part
		Part	:Lava_dropsLargerHit.part
  		Part    :Lava_GlobsLargerHit.part
		Part    :Lava_dropsBlackHit.part
		Part    :Lava_GlobBlackHit.part
		Part    :Lava_dropsBlackBigHit.part
	End


End

#############################################################

End
