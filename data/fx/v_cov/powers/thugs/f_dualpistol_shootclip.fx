#########################################################
##	Dual Pistols AoE
##

FxInfo

Flags InheritAlpha

#LifeSpan 400

##################################################################################
######  gun geo - Right first  ####################################################
###################################################################################

Condition
	On 	Time
	Time 	16

	Event
		Type	Local
		At	origin
		Sound uziload1 70 70 .45
	End
End



Condition
	On	Time
	Time	0

	Event
		EName	Gun_R
		Type	Local					
		At	WepR
		BhvrOverride
			FadeInLength		20
		End

		Geom	GEO_MicroUzi
	End

	
	Event
		EName	Clip_R
		Type	Local
		At	Gun_R
		 AltPiv	2
		BhvrOverride
			FadeInLength		20
		End

		Geom	GEO_MicroUzi_Clip
	End

#####  gun geo left  #####

	Event
		EName	Gun_L
		Type	Local					
		At	WepL
		BhvrOverride
			PyrRotate		0 0 180
			FadeInLength		20
		End

		Anim	FX_Gun_Uzi
	End
End


End


