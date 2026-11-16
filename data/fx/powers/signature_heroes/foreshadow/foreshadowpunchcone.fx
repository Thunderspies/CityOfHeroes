#########################################################
##	Fireweapon
########################################################
FxInfo

LifeSpan	145
######################################################################

Condition	
	On	time
	Time	0

	Event
		Type	Local
		At	Origin
		Sound PPfocus4b 150 150 .88
	End
End




Condition
	On 	Time
	Time 	35

	Event
		EName	Streak1
		Type	local
		At	UARMR
		
		Bhvr	Behaviors/GenericParticleFade.Bhvr

		Part	:StrengthTendrils.part
		Part	:StrengthTendrilsInvert.part
		Part	:Glows.part
		Part	:StrengthBodyGlows.part
		Part	:StrengthBodyGlows1.part
		Part	:StrengthBodyGlows3.part
		Part	:StrengthBodyGlows4.part
		PMagnet	LarmR
		
		LifeSpan	26
	
	End
	
	Event
		EName	Streak1
		Type	local
		At	UARML
		
		Bhvr	Behaviors/GenericParticleFade.Bhvr

		Part	:StrengthTendrils.part
		Part	:StrengthTendrilsInvert.part
		Part	:Glows.part
		Part	:StrengthBodyGlows.part
		Part	:StrengthBodyGlows1.part
		Part	:StrengthBodyGlows3.part
		Part	:StrengthBodyGlows4.part
		PMagnet	LarmL

		LifeSpan	26
	End

	Event
		EName	Streak1
		Type	local
		At	LarmR

		Bhvr	Behaviors/GenericParticleFade.Bhvr

		Part	:StrengthTendrils.part
		Part	:StrengthTendrilsInvert.part
		Part	:Glows.part
		Part	:StrengthBodyGlows.part
		Part	:StrengthBodyGlows1.part
		Part	:StrengthBodyGlows3.part
		Part	:StrengthBodyGlows4.part
		PMagnet	WepR

		LifeSpan	26
	End
	
	Event
		EName	Streak1
		Type	local
		At	LarmL

		Bhvr	Behaviors/GenericParticleFade.Bhvr

		Part	:StrengthTendrils.part
		Part	:StrengthTendrilsInvert.part
		Part	:Glows.part
		Part	:StrengthBodyGlows.part
		Part	:StrengthBodyGlows1.part
		Part	:StrengthBodyGlows3.part
		Part	:StrengthBodyGlows4.part
		PMagnet	WepL

		LifeSpan	26
	End

End

###########################
##Rings
#####################

Condition
	On 	Time
	Time 	48

	Event
		EName	Streak1
		Type	local
		At	WepR
		
		BhvrOverride
			PositionOffset		0 -.5 0
			PyrRotate	0 0 -30
		End

		Part2	:StrengthRingKO.part
		Part2	:StrengthRingBurst.part
		
	End

	Event
		EName	Streak1
		Type	local
		At	WepL
		
		BhvrOverride
			PositionOffset		0 -.5 0
			PyrRotate	0 0 30
		End

		Part2	:StrengthRingKO2.part
		Part2	:StrengthRingBurst2.part
		
	End

End

Condition
	On 	Time
	Time 	50	#49

	Event
		EName	Streak1
		Type	local
		At	WepR
		
		BhvrOverride
			PositionOffset		0 -.5 0
			PyrRotate	0 0 -30
		End

		Part2	:StrengthRingKO.part
		Part2	:StrengthRingBurst.part
		
	End

	Event
		EName	Streak1
		Type	local
		At	WepL
		
		BhvrOverride
			PositionOffset		0 -.5 0
			PyrRotate	0 0 30
		End

		Part2	:StrengthRingKO2.part
		Part2	:StrengthRingBurst2.part
		
	End

End


###########################
##Geom  
#####################

Condition
	On 	Time
	Time 	52

	Event
		EName	Streak1
		Type	start
		At	BendMystic
		
		BhvrOverride
			StartColor		100 100 255
			PositionOffset		0 -.5 0
		End

		Bhvr	Behaviors/ForeShadowFade2.bhvr
		Geom	ForeShadowBlast2
		
		LifeSpan	5
	End

End

#############################
####Blasts
#######################

Condition
	On 	Time
	Time 	52

	Event
		EName	Prime
		Type	start
		At	BendMystic
		
		Bhvr	Behaviors/ForeShadowBlurProjectiles.bhvr
		
		Part	:BlastTendrilsInvert.part
		Part	:BlastTendrils.part
		Part	:BlastTendrilsInvertBright.part
		Part	:BlastTendrilsBright.part

		Magnet	T_Chest

	End

End

Condition
	On 	PrimeHit
	
	Event
		EName 	Prime
		Type	Destroy
		
	End

End

End
##################################

