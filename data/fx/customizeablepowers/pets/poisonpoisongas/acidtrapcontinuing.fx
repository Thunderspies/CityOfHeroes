#############################################################
## AcidTrapContinuing.fx
#############################################################

FxInfo

#############################################################

Condition
	On 	Time
	Time 	20

	Event
		EName 	arrow
		Type	start
		At	Root

		BHVR	Weapons\bow\trickarrow\Offset.bhvr
		Part	:AcidFlat2.part
		Part	:AcidFlatDark2.part
		Part	:AcidBubbles2.part
		Part	:AcidBubblesdark2.part
		Part	:AcidBubble2.part
	End
End

#########################################################

End

