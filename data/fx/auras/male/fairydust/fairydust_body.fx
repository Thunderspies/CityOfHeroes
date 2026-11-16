#############################################################
## FairyDust_Body.fx
#############################################################

FxInfo
Flags	DontSuppress

#########################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	PositOnly
		At	Chest
		Part	:Smoke_Trail_Body.part
		Part	:Smoke_Stationary_Body.part
		Part	:Smoke_Stationary_Body_small.part

		Part	:Dust_Trail_Body_Big.part
		POther	Hips

	End

	Event
		Type	PositOnly
		At	Hair
		Part	:Dust_Idle_Body.part
		Part	:Dust_Idle_Body_Big.part
		POther	Root
	End

	Event
		Type	PositOnly
		At	Head
		Part	:Smoke_Trail_Head.part
		Part	:Smoke_Stationary.part
		Part	:Dust_Trail_Light.part

	End

	Event
		Type	PositOnly
		At	WepL
		Part	:Smoke_Stationary.part
		Part	:Dust_Trail_Light.part
		Part	:Smoke_Trail.part
		#Part	:Dust_Stationary.part
	End

	Event
		Type	PositOnly
		At	WepR
		Part	:Smoke_Stationary.part
		Part	:Dust_Trail_Light.part
		Part	:Smoke_Trail.part
	#	Part	:Dust_Stationary.part
	End

	Event
		Type	PositOnly
		At	FootL
		Part	:Smoke_Trail.part
		Part	:Dust_Trail_Light.part
	End

	Event
		Type	PositOnly
		At	FootR
		Part	:Smoke_Trail.part
		Part	:Dust_Trail_Light.part
	End

End

Condition
	On	Cycle
	Time	5
	Chance	0.75

	Event
		Type	PositOnly
		At	WepR
		ChildFX	:CHILD_Dust_Falling.fx
		Lifespan 30
	End

	Event
		Type	PositOnly
		At	WepL
		ChildFX	:CHILD_Dust_Falling.fx
		Lifespan 30
	End
End

#########################################################

End