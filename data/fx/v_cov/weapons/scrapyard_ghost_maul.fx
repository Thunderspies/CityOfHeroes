#########################################################
##	jack
##

FxInfo

Flags InheritAlpha DontSuppress

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		EName 	Prime
		Type	Local
		At	WepR
		Geom	GEO_WepR_Sledge_Hammer
		Bhvr	:WeaponScrapyardGhostMaul.bhvr
		Part	:GhostMaulGlow.part
		Sound axeout 70 70 .5
	End

	Event
		EName 	Glow
		Type	Local
		At	WepR
		Bhvr	:GhostMaulHeadOffset.bhvr
		Part	:GhostMaulHeadGlow.part
	End
End

End