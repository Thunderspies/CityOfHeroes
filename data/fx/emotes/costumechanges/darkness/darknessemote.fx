#############################################################
## DarknessEmote.fx
#############################################################

FxInfo

Lifespan 120
#############################################################
##################    AUDIO   ###############################
#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	root
	#	Sound	Ice7 100 80 .6
	End
End

#############################################################
##################    GROUND TENTICLES   ####################
#############################################################
Condition
	On 	Time
	Time 	0

	Event
		Type	Start
		At	Root
		BhvrOverride
			PositionOffset 0 0.0 0
		End
		Part	:GroundTendrils.part
		Lifespan	100
	End
End

Condition
	On 	Time
	Time 	15

	Event
		Type	Start
		At	Root
		BhvrOverride
			PositionOffset	0 0.0 0
		End
		Part	:CreateDarknessPuddleFlat.part
		Part	:CreateDarknessPuddle2Flat.part
		Part	:CreateDarknessPuddle.part
		Part	:CreateDarknessPuddle2.part
		Lifespan	40
	End

End

#############################################################
##################    BODY SHROUD   #########################
#############################################################

Condition
	On 	Time
	Time 	25
## DarknessEmote.fx
#############################################################



	Event
		Type	posit
		At	Root
		ChildFX	:BodyTenticles.fx
		Lifespan	80
	End

End

Condition
	On 	Time
	Time 	55

	Event
		Type	posit
		At	Root
		ChildFX	:BodyShroud.fx
		Lifespan	37
	End
End

#############################################################
##################   END   ##################################
#############################################################