#########################################################
##
##

FxInfo


Input
	InpName	Origin
End

Lifespan 100


Condition
	On Time
	Time 5
	Event
		Type	Local
		At	origin
		Sound Emote_Cast_01 90 90 1.0
	End
End

Condition
	On	Time
	Time	3

	Event
		Ename 	InitialGlow
		Type	Local
		At	WepL
		part	:InitialLight.part
		Lifespan 70
	End

End

Condition
	On	Time
	Time	15

	Event
		Ename	BigGlow
		Type	Local
		At	WepL
		part	Emotes\CostumeChanges\Laugh\InitialLight.part
	End

End

Condition
	On	Time
	Time	30
	Event
		Ename	BodyGlow
		Type	Local
		At	Root
		ChildFX	Emotes\CostumeChanges\BodyGlow\BodyGlow.fx
	End
End

Condition
	On 	Time
	Time 	32

	Event
		EName 	Smoke
		Type	Start
		At	Root
		part	:SmokePillar.part
		part	:SmokePillar2.part
		part	:SmokePillarAdditive.part
	End

	Event
		Ename 	Flash
		Type	Local
		At	WepL
		Part	Emotes\CostumeChanges\Lightning\LargeGlow.part 	#big glow
	End



#################################
######### ROCKET 1 ##############
#################################
#
#	Event
#		Ename	Rocket1
#		Type	Local
#		At 	Root
#		Bhvr 	behaviors\FireBurstUP.bhvr
#		Lifespan 50
#
#	End
#
#
#	Event
#		Ename 	Spinner1
#		Type	Local
#		At	Rocket1
#		BhvrOverride
#			Spin 0.0 0.8 0.0
#		End
#		Lifespan 50
#	End
#
#	Event
#		Ename 	Offset
#		Type 	Local
#		At	Spinner1
#		BhvrOverride
#			PositionOffset 0.0 0.0 1.0
#		End
#	#	Part	:Sparks.part
#		Lifespan 50
#	End
#End