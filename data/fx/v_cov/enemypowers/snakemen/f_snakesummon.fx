
FxInfo

#Flags InheritAnimScale

Lifespan	280


Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	Origin
		Sound Rumble2 80 80 .6
	End
	
#############################################################
##Targets

	Event
		EName	T_C_01
		Type	create		
		Inherit	Position Scale

		At	T_Root
		BhvrOverride

			StartJitter	5 0 5
		End
		
	End

	Event
		EName	T_C_01
		Type	create		
		Inherit	Position Scale

		At	T_Root
		BhvrOverride

			StartJitter	5 0 5
		End
		
	End
End

#######################################################################

Condition
	On	Time
	Time	12

	Event
		Type	Local
		At	Origin
		Sound snakeSummon1 80 80 1.0
	End
End



#####  BodyBag  #######################################

Condition
	On	Time
	Time	10
Repeat	1

	Event
		EName	ChildSnake
		Type	Start
		At	origin

		ChildFX	:F_SnakeSummon_child.fx
	End
End

Condition
	On	Time
	Time	15
Repeat	1

	Event
		EName	ChildSnake02
		Type	Start
		At	origin

		ChildFX	:F_SnakeSummon_child.fx
	End
End

Condition
	On	Time
	Time	20
Repeat	1

	Event
		EName	ChildSnake03
		Type	Start
		At	origin

		ChildFX	:F_SnakeSummon_child.fx
	End
End

#########################  Single Snake At Target  ################################

Condition
	On	Time
	Time	5

	Event
		EName	Prime
		Type	create		
		Inherit	Position Rotation
		At	Root

		bhvrOverride
			TrackRate	.5
			StartJitter		5.5 0.0 5.5
#			Scale			.3 .3 .3
#			FadeInLength		150
#			FadeOutLength		50
		End

		bhvr	:B_SnakeSummonFade.bhvr
		Anim	FX_Snake		

		Magnet	T_C_01
	End
End


##########################  Kill Snake 01  ################################

Condition
	On	PrimeHit
	
	Event
		EName	Prime
		Type	Destroy
	End
	
End

#################################################################################
#################################################################################
#################################################################################
#################################################################################

Condition
	On	Time
	Time	7

	Event
		EName	Prime2
		Type	create		
		Inherit	Position Rotation
		At	Root

		bhvrOverride
			TrackRate	.75
			StartJitter		5.5 0.0 5.5
			Scale			.95 .95 .95
#			FadeInLength		150
#			FadeOutLength		50
		End

		bhvr	:B_SnakeSummonFade.bhvr
		Anim	FX_Snake		

		Magnet	T_Root
	End
End

Condition
	On	Prime2Hit
	
	Event
		EName	Prime2
		Type	Destroy
	End
	
End

##################################################################################
##################################################################################
##################################################################################
##################################################################################

Condition
	On	Time
	Time	13

	Event
		EName	Prime3
		Type	create		
		Inherit	Position Rotation
		At	Root

		bhvrOverride
			TrackRate	.65
			StartJitter		5.5 0.0 5.5
			Scale			1.1 .1 .1
#			FadeInLength		150
#			FadeOutLength		50
		End

		bhvr	:B_SnakeSummonFade.bhvr
		Anim	FX_Snake		

		Magnet	T_Root
	End
End


Condition
	On	Prime3Hit
	
	Event
		EName	Prime3
		Type	Destroy
	End
	
End

#########################  Single Snake At Target  ################################

Condition
	On	Time
	Time	17

	Event
		EName	Prime4
		Type	create		
		Inherit	Position Rotation
		At	Root

		bhvrOverride
			TrackRate	.55
			StartJitter		5.5 0.0 5.5
#			Scale			.65 .65 .65
#			FadeInLength		150
#			FadeOutLength		50
		End

		bhvr	:B_SnakeSummonFade.bhvr
		Anim	FX_Snake		

		Magnet	T_Root
	End
End

Condition
	On	Prime4Hit
	
	Event
		EName	Prime4
		Type	Destroy
	End
	
End

#################################################################################
#################################################################################
#################################################################################
#################################################################################

#########################  Single Snake At Target  ################################

Condition
	On	Time
	Time	17

	Event
		EName	Prime5
		Type	create		
		Inherit	Position Rotation
		At	Root

		bhvrOverride
			TrackRate	.45
			StartJitter		5.5 0.0 5.5
#			Scale			.65 .65 .65
#			FadeInLength		150
#			FadeOutLength		50
		End

		bhvr	:B_SnakeSummonFade.bhvr
		Anim	FX_Snake		

		Magnet	T_Root
	End
End

Condition
	On	Prime5Hit

	Event
		EName	Prime5
		Type	Destroy
	End
	
End

End		