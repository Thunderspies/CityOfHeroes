#########################################################
##	template

FxInfo

#########################################################
#################### GEYSER ########################
#########################################################

Condition
	On	Time
	Time	0

	Event
		Ename	InitialSteam
		Type	StartPositOnly
		At	Root

	#	Part	:InitialSteam.part
	#	Part	:InitialViolentSteam.part
	#	Part 	Emotes\CostumeChanges\Lightning\LowerBlast.part

		Lifespan 10
	End


	Event
		Ename	GeyserHighlights
		Type	StartPositOnly
		At	Root
		BhvrOverride
			PositionOffset	0.0 -13.0 0.0
		End
		Part	:GeyserHighlightsArea.part

		Lifespan 40
	End


	Event
		Ename 	Flash
		Type	StartPositOnly
		At	Root

	#	Part	:Glow2.part
	#	Part	:LavaBubblePopping.part
		Part	:GasRadiusRocksBig.part
		Part	:LavaSpray2.part
		Lifespan 50

	End

	Event
		Ename	GroundSmoke
		Type	StartPositOnly
		At	Root

		Part	:VolcanicDustBig.part
		Part	:VolcanicDustBig2.part

		Lifespan 58
	End



	Event
		Ename 	Crack
		Type	StartPositOnly
		At	Root
		Bhvr	:Splat_FootStomp.bhvr
		BhvrOverride
			PyrRotateJitter 0 0 0
			StartColor 250 200 150
			SplatFlags ADDITIVE
			Scale 22 0.0 22
		End
		Splat	crackedEarth.tga
		Lifespan 60

	End

End


Condition
	On	Time
	Time	5




	Event
		Ename	Steam
		Type	StartPositOnly
		At	Root
		BhvrOverride
			PositionOffset	0.0 1.0 0.0
			Scale		1.25 1.25 1.25
		End
	#	Part	:Steam.part
		Part	:SteamAlpha.part
		Lifespan 45
	End

	Event
		Ename	GeyserCore
		Type	StartPositOnly
		At	Root
		BhvrOverride
			PositionOffset	0.0 -1.0 0.0
			Scale		1.25 1.25 1.25

		End
		Part	:ViolentSteamCoreAdd.part

		Lifespan 20
	End

		Event
		Ename	GeyserCore2
		Type	StartPositOnly
		At	Root
		BhvrOverride
			PositionOffset	0.0 -1.0 0.0
			Scale		1.25 1.25 1.25

		End
		Part	:ViolentSteamCoreBlue.part

		Lifespan 25
	End
End

#########################################################
#################### SMALL GEYSERS ######################
#########################################################


#Condition
#	On	Time
#	Time 	7
#	Repeat 	2
#
#
#	Event
#		Ename	SmallGeysers
#		Type	Start
#		ChildFX	:GeyserStarters.fx
#		Lifespan 30
#	End
#End
#



Condition
	On 	Time
	Time 	300

	Event
		EName	All
		Type	Destroy

	End


End






























