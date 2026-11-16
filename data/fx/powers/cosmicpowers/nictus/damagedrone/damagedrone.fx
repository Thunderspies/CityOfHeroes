#########################################################
## Umbral Aura - Orbitting Death
#########################################################

FxInfo

## SOUND FX #######################################################

Condition
	On	Time
	Time	10

	Event
		Type	Local	
		At	root
		Sound warshadedamdrone_loop 80 80 .66
		bhvr	behaviors/soundFade3.bhvr
		Flags	PowerLoopingSound
		Lifespan 500
	End
End

## EYE FX #######################################################

#Condition
#	On	Time
#	Time	0
#
#	Event
#		EName 	Eyes
#		Type	Local 
#		At	Root
#		ChildFX POWERS\CosmicPowers\Nictus\ActivationEyes\ActivationEyesContinuingFX.fx
#	End
#End

## HANDS #######################################################

Condition
	On	Time
	Time	10

	Event
		Type	Local
		At	WepL

		part	:DroneHandGlows.part
		part	:DroneHandGlowsPurple.part
		part	:DroneHandGlowsBlue.part
		part	:Sparks.part
		bhvr	Behaviors/GenericParticleFade.bhvr
		LifeSpan	75
	End
	
	Event
		Type	Local
		At	WepR

		part	:DroneHandGlows.part
		part	:DroneHandGlowsPurple.part
		part	:DroneHandGlowsBlue.part
		part	:Sparks.part
		bhvr	Behaviors/GenericParticleFade.bhvr
		LifeSpan	75
	End
End

## GROUND FX #######################################################

Condition
	On	Time
	Time	10

	Event
		Type	Local	
		At	root
		Pmagnet	Chest

		part	:BaseFlatTendrils.part
		part	:BaseFlatTendrilsPurple.part
		part	:BaseFlatTendrilsBlue.part
		#ChildFx	:DamageDroneBaseTendrils.fx
	End
End

##Condition
##	On	Time
##	Time	10
##
##	Event
##		Ename	fireSound
##		Type	Local	
##		At	root
##
##		Geom	HotFeet01
##		Bhvr	:Scale.bhvr
##	End
##End
##
##Condition
##	On 	cycle
##	Time 	20
##	Chance	.75
##
##	Event
##		EName	Fire2
##		Type	start
##		At	firesound
##		altpiv	2
##		
##		part	:BlackTendrilsRing.part
##		part	:PurpleTendrilsRing.part
##		part	:BlueTendrilsRing.part	
##		part	:FlatTendrils.part
##		part	:FlatTendrilsPurple.part
##		part	:FlatTendrilsBlue.part
##
##		Lifespan	10
##	End
##End
##
##Condition
##	On 	cycle
##	Time 	22
##	Chance	.75
##
##	Event
##		EName	Fire11
##		Type	start
##		At	firesound
##		altpiv	12
##
##		part	:BlackTendrilsRing.part
##		part	:PurpleTendrilsRing.part
##		part	:BlueTendrilsRing.part	
##		part	:FlatTendrils.part
##		part	:FlatTendrilsPurple.part
##		part	:FlatTendrilsBlue.part
##			
##		Lifespan	10		
##		LifespanJitter	2		
##	End
##End
##
##Condition
##	On 	cycle
##	Time 	24
##	Chance	.75
##
##	Event
##		EName	Fire11
##		Type	start
##		At	firesound
##		altpiv	13
##
##		part	:BlackTendrilsRing.part
##		part	:PurpleTendrilsRing.part
##		part	:BlueTendrilsRing.part	
##		part	:FlatTendrils.part
##		part	:FlatTendrilsPurple.part
##		part	:FlatTendrilsBlue.part
##			
##		Lifespan	10		
##		LifespanJitter	2		
##	End
##End
##
##Condition
##	On 	cycle
##	Time 	26
##	Chance	.75
##
##	Event
##		EName	Fire7
##		Type	start
##		At	firesound
##		altpiv	7
##
##		part	:BlackTendrilsRing.part
##		part	:PurpleTendrilsRing.part
##		part	:BlueTendrilsRing.part	
##		part	:FlatTendrils.part
##		part	:FlatTendrilsPurple.part
##		part	:FlatTendrilsBlue.part
##
##		Lifespan	10		
##		LifespanJitter	2		
##	End
##End
##
##Condition
##	On 	cycle
##	Time 	28
##	Chance	.75
##
##	Event
##		EName	Fire8
##		Type	start
##		At	firesound
##		altpiv	8
##
##		part	:BlackTendrilsRing.part
##		part	:PurpleTendrilsRing.part
##		part	:BlueTendrilsRing.part	
##		part	:FlatTendrils.part
##		part	:FlatTendrilsPurple.part
##		part	:FlatTendrilsBlue.part
##
##		Lifespan	10		
##		LifespanJitter	2		
##	End
##End

## ORBITTING DRONES #######################################################

Condition
	On	Time
	Time	25

	Event
		Ename	Drone2
		Type	posit
		At	Root
		
		BHVR	:DamageDroneOpposite.bhvr
	End

	Event
		Ename	DroneSpin2
		Type	local
		At	Drone2
	
		BHVR	:DamageDroneSpinOppositeSML.bhvr
		Part	:BLKTendrils.part
		Part	:BLKTendrilsTrail.part
		#Part	:Light.part
		Part	:PurpleTendrils.part
		Part	:BlueTendrils.part
		Part	:PurpleTendrilsTrail.part
		Part	:BlueTendrilsTrail.part
		#Part	:Sparks.part
	End
End

##Condition
##	On	Time
##	Time	30
##
##	Event
##		Ename	Drone
##		Type	posit
##		At	Root
##		
##		BHVR	:DamageDrone.bhvr
##	End
##
##	Event
##		Ename	DroneSpin
##		Type	local
##		At	Drone
##		
##		BHVR	:DamageDroneSpin.bhvr
##		Part	:BLKTendrils.part
##		Part	:BLKTendrilsTrail.part
##		#Part	:Light.part
##		Part	:PurpleTendrils.part
##		Part	:BlueTendrils.part
##		Part	:PurpleTendrilsTrail.part
##		Part	:BlueTendrilsTrail.part
##		#Part	:Sparks.part
##	End
##End

Condition
	On	Time
	Time	45
	
	Event
		Ename	Drone3
		Type	posit
		At	Root
		
		BHVR	:DamageDroneOppositeSML.bhvr
	End

	Event
		Ename	DroneSpin3
		Type	local
		At	Drone3
		
		BHVR	:DamageDroneSpinOppositeSML.bhvr
		Part	:BLKTendrils.part
		Part	:BLKTendrilsTrail.part
		#Part	:Light.part
		Part	:PurpleTendrils.part
		Part	:BlueTendrils.part
		Part	:PurpleTendrilsTrail.part
		Part	:BlueTendrilsTrail.part
		#Part	:Sparks.part
	End
End

Condition
	On	Time
	Time	53

	Event
		Ename	Drone4
		Type	posit
		At	Root
		
		BHVR	:DamageDroneLGE.bhvr
	End

	Event
		Ename	DroneSpin4
		Type	local
		At	Drone4
		
		BHVR	:DamageDroneSpinLGE.bhvr
		Part	:BLKTendrils.part
		Part	:BLKTendrilsTrail.part
		#Part	:Light.part
		Part	:PurpleTendrils.part
		Part	:BlueTendrils.part
		Part	:PurpleTendrilsTrail.part
		Part	:BlueTendrilsTrail.part
		#Part	:Sparks.part
	End
End    

#########################################################

End