#############################################################
## FairyDust_BodyAndEyes.fx
#############################################################

FxInfo
Flags	DontSuppress

#########################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	Origin
		ChildFX	:FairyDust_Eyes.fx

	End

	Event
		Type	PositOnly
		At	Chest
		Part	:Smoke_Trail_Body.part
		Part	:Smoke_Stationary_Body.part

	End

	Event
		Type	PositOnly
		At	Hair
		Part	:Dust_Trail_Body.part
		Part	:Dust_Trail_Body_Big.part
		POther	Root
	End

	Event
		Type	PositOnly
		At	Head
		Part	:Smoke_Trail_Head.part
		Part	:Dust_Trail_Light.part

	End

	Event
		Type	PositOnly
		At	WepL
		Part	:Smoke_Stationary.part
		Part	:Dust_Trail_Light.part
		Part	:Smoke_Trail.part
		Part	:Dust_Stationary.part
	End

	Event
		Type	PositOnly
		At	WepR
		Part	:Smoke_Stationary.part
		Part	:Dust_Trail_Light.part
		Part	:Smoke_Trail.part
		Part	:Dust_Stationary.part
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

#########################################################

End