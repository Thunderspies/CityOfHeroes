#############################################################
## TarotCards_Hit.fx
#############################################################

FxInfo

Lifespan 1500


#############################################################

Condition
	On	Time
	Time	20

	Event
		Type	Local
		At 	Origin
		Sound Tarot_Hit_01 80 80 0.8
	End
End

#############################################################

#############################################################

Condition
	On	Time
	Time	0

	Event
		EName	HookupRootToChest
		Type	Local
		At	Root
		Geom	RootToChestAdjustment
	End

	Event
		EName	HookupOrigin
		Type	Local
		At	HookupRootToChest
		AltPiv	1
		BhvrOverride
			PositionOffset		0 0.5 0
		End
	End
End

#############################################################

Condition
	On	Time
	Time	0
	Repeat	3

	Event
		Type	Local
		At	HookupOrigin
		ChildFX	:Child_CardsSwirlIn.fx
		Lifespan 150
	End
End

#############################################################

End