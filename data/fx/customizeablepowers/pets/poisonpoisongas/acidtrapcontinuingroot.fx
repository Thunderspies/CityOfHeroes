#############################################################
## AcidTrapContinuingRoot.fx
#############################################################

FxInfo

#########################################################

Input
	Inpname	Hips
End

#########################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	start
		At	Root
		ChildFX	:PoisonAOEGassesExplode.fx
	End

	Event
		Type	start
		At	Root
		BhvrOverride
			PositionOffset		0 3 0
		End
		Part	Weapons\Bow\Trickarrow\AcidFrontFacing.part
		Part	Weapons\Bow\Trickarrow\Acid2Light.part
		Part	Weapons\Bow\Trickarrow\AcidDark2.part
		Part	Weapons\Bow\Trickarrow\AcidBubbles2.part
		Part	Weapons\Bow\Trickarrow\AcidBubblesdark2.part
		Part	Weapons\Bow\Trickarrow\AcidBubble2.part
	End
End

#############################################################

End