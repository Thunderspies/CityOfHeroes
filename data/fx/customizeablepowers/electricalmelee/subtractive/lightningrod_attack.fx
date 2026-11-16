#############################################################
## Red_LightningRodCast.fx
#############################################################

FxInfo

Lifespan 165

## FIRST LIGHTNING STRIKE ###########################################################

Condition
	On 	Time
	Time 	30

	Event
		Type	local
		At	Root
		ChildFx	:Child_LightningRod.fx
	End
End

Condition
	On 	Time
	Time 	30

	Event
		Type	local
		At	Chest
		Part	:LightningStrikeGlow.part
		Part	:LightningStrikeHaze.part
		Part	:LightningStrikeHazeWhite.part
		Lifespan  10
	End
End

Condition
	On 	Time
	Time 	28

	Event
		Type	local
		At	Chest
		Part	:LightningStreaks.part
		Part	:LightningStreaks2.part
		Lifespan  10
	End
End

## SECOND LIGHTNING STRIKE ###########################################################

Condition
	On 	Time
	Time 	46

	Event
		Type	local
		At	Root
		ChildFx	:Child_LightningRod.fx
	End
End

Condition
	On 	Time
	Time 	46

	Event
		Type	local
		At	Chest
		Part	:LightningStrikeGlow.part
		Part	:LightningStrikeHaze.part
		Part	:LightningStrikeHazeWhite.part
		Lifespan  10
	End
End

## PBAOE ###########################################################

Condition
	On 	Time
	Time 	46

	Event
		Type	local
		At	Chest
		ChildFx	:Child_ThunderStomp.fx
	End
End

#############################################################

End