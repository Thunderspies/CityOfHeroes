#########################################################
##	Fireweapon
########################################################
FxInfo

LifeSpan	45
######################################################################

Condition
	On 	Time
	Time 	5

#####################################################
##Flash
################################

	Event
		EName	Streak1
		Type	local
		At	chest

		Bhvr	Behaviors/GenericParticlefade.bhvr

		Part	:HitTendrilsInvert.part
		Part	:HitTendrils.part
		Sound eleclaserhit 80 80 .6
		LifeSpan	5
	
	End

End

#############################################################
##Glow
##############################

Condition
	On 	Time
	Time 	0

	Event
		EName	Streak1
		Type	local
		At	chest

		Bhvr	Behaviors/GenericParticlefade.bhvr

		Part	:HitThinRing.part

		LifeSpan	10
	
	End

	Event
		EName	Streak1
		Type	local
		At	chest

		Bhvr	Behaviors/GenericParticlefade.bhvr

		Part	:HitFlash.part
		Part	:HitFlashBubble.part
		LifeSpan	10
	
	End

	Event
		EName	Streak1
		Type	local
		At	chest

		Bhvr	Behaviors/GenericParticlefade.bhvr

		Part	:HitRing.part
		
#		LifeSpan	10
	
	End

	Event
		EName	Streak1
		Type	local
		At	chest

		Bhvr	Behaviors/GenericParticlefade.bhvr

		Part	:HitTendrilsInvert.part
		Part	:HitTendrils.part
		
		LifeSpan	5
	
	End

	Event
		EName	Streak1
		Type	local
		At	hair

		Bhvr	Behaviors/GenericParticlefade.bhvr

		Part	:StrengthTendrils.part
		Part	:StrengthTendrils.part
		Part	:StrengthTendrilsInvert.part
		Part	:Glows.part
		Part	:StrengthBodyGlows.part
		Part	:StrengthBodyGlows1.part
		Part	:StrengthBodyGlows3.part
		Part	:StrengthBodyGlows4.part
		PMagnet	neck

		LifeSpan	20
	
	End

	Event
		EName	Streak1
		Type	local
		At	UARMR

		Bhvr	Behaviors/GenericParticlefade.bhvr

		Part	:StrengthTendrils.part
		Part	:StrengthTendrils.part
		Part	:StrengthTendrilsInvert.part
		Part	:Glows.part
		Part	:StrengthBodyGlows.part
		Part	:StrengthBodyGlows1.part
		Part	:StrengthBodyGlows3.part
		Part	:StrengthBodyGlows4.part
		PMagnet	LarmR

		LifeSpan	20
	
	End
	
	Event
		EName	Streak1
		Type	local
		At	UARML

		Bhvr	Behaviors/GenericParticlefade.bhvr

		Part	:StrengthTendrils.part
		Part	:StrengthTendrils.part
		Part	:StrengthTendrilsInvert.part
		Part	:Glows.part
		Part	:StrengthBodyGlows.part
		Part	:StrengthBodyGlows1.part
		Part	:StrengthBodyGlows3.part
		Part	:StrengthBodyGlows4.part
		PMagnet	LarmL

		LifeSpan	20
	End

	Event
		EName	Streak1
		Type	local
		At	LarmR

		Bhvr	Behaviors/GenericParticlefade.bhvr

		Part	:StrengthTendrils.part
		Part	:StrengthTendrils.part
		Part	:StrengthTendrilsInvert.part
		Part	:Glows.part
		Part	:StrengthBodyGlows.part
		Part	:StrengthBodyGlows1.part
		Part	:StrengthBodyGlows3.part
		Part	:StrengthBodyGlows4.part
		PMagnet	WepR

		LifeSpan	20
	End
	
	Event
		EName	Streak1
		Type	local
		At	LarmL

		Bhvr	Behaviors/GenericParticlefade.bhvr

		Part	:StrengthTendrils.part
		Part	:StrengthTendrils.part
		Part	:StrengthTendrilsInvert.part
		Part	:Glows.part
		Part	:StrengthBodyGlows.part
		Part	:StrengthBodyGlows1.part
		Part	:StrengthBodyGlows3.part
		Part	:StrengthBodyGlows4.part
		PMagnet	WepL

		LifeSpan	20
	End

##########################################################################3
##Lowerbody
#############################

	Event
		EName	Streak1
		Type	local
		At	UlegR

		Bhvr	Behaviors/GenericParticlefade.bhvr

		Part	:StrengthTendrils.part
		Part	:StrengthTendrils.part
		Part	:StrengthTendrilsInvert.part
		Part	:Glows.part
		Part	:StrengthBodyGlows.part
		Part	:StrengthBodyGlows1.part
		Part	:StrengthBodyGlows3.part
		Part	:StrengthBodyGlows4.part
		PMagnet	LlegR

		LifeSpan	20
	
	End
	
	Event
		EName	Streak1
		Type	local
		At	UlegL

		Bhvr	Behaviors/GenericParticlefade.bhvr

		Part	:StrengthTendrils.part
		Part	:StrengthTendrils.part
		Part	:StrengthTendrilsInvert.part
		Part	:Glows.part
		Part	:StrengthBodyGlows.part
		Part	:StrengthBodyGlows1.part
		Part	:StrengthBodyGlows3.part
		Part	:StrengthBodyGlows4.part
		PMagnet	LlegL

		LifeSpan	20
	End

	Event
		EName	Streak1
		Type	local
		At	LlegR

		Bhvr	Behaviors/GenericParticlefade.bhvr

		Part	:StrengthTendrils.part
		Part	:StrengthTendrils.part
		Part	:StrengthTendrilsInvert.part
		Part	:Glows.part
		Part	:StrengthBodyGlows.part
		Part	:StrengthBodyGlows1.part
		Part	:StrengthBodyGlows3.part
		Part	:StrengthBodyGlows4.part
		PMagnet	FootR

		LifeSpan	20
	End
	
	Event
		EName	Streak1
		Type	local
		At	LlegL

		Bhvr	Behaviors/GenericParticlefade.bhvr

		Part	:StrengthTendrils.part
		Part	:StrengthTendrils.part
		Part	:StrengthTendrilsInvert.part
		Part	:Glows.part
		Part	:StrengthBodyGlows.part
		Part	:StrengthBodyGlows1.part
		Part	:StrengthBodyGlows3.part
		Part	:StrengthBodyGlows4.part
		PMagnet	FootL

		LifeSpan	20
	End

	Event
		EName	Streak1
		Type	local
		At	FootR

		Bhvr	Behaviors/GenericParticlefade.bhvr

		Part	:StrengthTendrils.part
		Part	:StrengthTendrilsInvert.part
		Part	:Glows.part
		Part	:StrengthBodyGlows.part
		Part	:StrengthBodyGlows1.part
		Part	:StrengthBodyGlows3.part
		Part	:StrengthBodyGlows4.part

		LifeSpan	20
	End
	
	Event
		EName	Streak1
		Type	local
		At	FootL

		Bhvr	Behaviors/GenericParticlefade.bhvr

		Part	:StrengthTendrils.part
		Part	:StrengthTendrilsInvert.part
		Part	:Glows.part
		Part	:StrengthBodyGlows.part
		Part	:StrengthBodyGlows1.part
		Part	:StrengthBodyGlows3.part
		Part	:StrengthBodyGlows4.part

		LifeSpan	20
	End

End


End

