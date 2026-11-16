#############################################################
## StoneFist_Activation.fx
#############################################################

FxInfo

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	LARMR
		Bhvr	CustomizeablePowers\StoneMelee\Lava\MagmaHands.bhvr
		geom	Male_MagmaHandR
	End

	Event
		Type	Local
		At	LARML
		Bhvr	CustomizeablePowers\StoneMelee\Lava\MagmaHands.bhvr
		geom	Male_MagmaHandL
	End

	Event
		Type	Local
		At	LARMR
		Bhvr	CustomizeablePowers\StoneMelee\Lava\MagmaHands.bhvr
		geom	Male_MagmaHandR_AddGlow
	End

	Event
		Type	Local
		At	LARML
		Bhvr	CustomizeablePowers\StoneMelee\Lava\MagmaHands.bhvr
		geom	Male_MagmaHandL_AddGlow
	End
End

#############################################################

End