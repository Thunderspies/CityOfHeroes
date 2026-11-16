#########################################################
## Umbral Aura - Gravity Shield
########################################################

FxInfo
	
## SOUND FX ######################################################

Condition
	On 	Time
	Time 	5

	Event
		Type	Local
		At	origin
		Sound WarshadeShield_loop 80 80 .37
		bhvr	behaviors/soundFade3.bhvr
		Flags	PowerLoopingSound
		Lifespan 20
				
	End
End

## EYE FX ######################################################

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

## AURA ######################################################

Condition
	On 	Time
	Time 	5
	
	Event
		Type	Posit
		At	HIPS
		Bhvr 	:NictusShieldBHVR.bhvr
		
		Geom	CosmicNictusShield
	End
End

Condition
	On 	Time
	Time 	5

#	Event
#		EName	Chest
#		Type	Local
#		At	Chest
#		
#		part	POWERS\CosmicPowers\Nictus\NictusCoreHips.part
#		part	POWERS\CosmicPowers\Nictus\NictusCoreEnergy.part
#		part	POWERS\CosmicPowers\Nictus\NictusCoreEnergy1.part
#		part	POWERS\CosmicPowers\Nictus\NictusCoreEnergyPurpleTop.part
#		part	POWERS\CosmicPowers\Nictus\NictusCoreEnergyTealTop.part
#	End

	Event
		EName	Chest
		Type	Local
		At	COL_L
		part	POWERS\CosmicPowers\Nictus\NictusCore2.part
		part	POWERS\CosmicPowers\Nictus\NictusCoreEnergy.part
		part	POWERS\CosmicPowers\Nictus\NictusCoreEnergy1.part
		part	POWERS\CosmicPowers\Nictus\NictusCoreEnergyPurpleTop.part
		part	POWERS\CosmicPowers\Nictus\NictusCoreEnergyTealTop.part
	End

	Event
		EName	Chest
		Type	Local
		At	COL_R
		part	POWERS\CosmicPowers\Nictus\NictusCore2.part
		part	POWERS\CosmicPowers\Nictus\NictusCoreEnergy.part
		part	POWERS\CosmicPowers\Nictus\NictusCoreEnergy1.part
		part	POWERS\CosmicPowers\Nictus\NictusCoreEnergyPurpleTop.part
		part	POWERS\CosmicPowers\Nictus\NictusCoreEnergyTealTop.part
		
	End

#	Event
#		EName	Chest
#		Type	Local
#		At	Waist
#		
#		part	POWERS\CosmicPowers\Nictus\NictusCore2.part
#		Part	POWERS\CosmicPowers\Nictus\NictusCoreSparks2.part
#		part	POWERS\CosmicPowers\Nictus\NictusCoreEnergyPurpleTop.part
#		part	POWERS\CosmicPowers\Nictus\NictusCoreEnergyTealTop.part
#		part	POWERS\CosmicPowers\Nictus\NictusCoreEnergyPurpleTop.part
#		part	POWERS\CosmicPowers\Nictus\NictusCoreEnergyTealTop.part
#	End

	Event
		EName	Hips2
		Type	Local
		At	Hips
		
		part	POWERS\CosmicPowers\Nictus\NictusCoreHips.part
		part	POWERS\CosmicPowers\Nictus\NictusCoreEnergyPurple.part
		part	POWERS\CosmicPowers\Nictus\NictusCoreEnergyTeal.part
	End

##	Event
##		EName	Lleg
##		Type	Local
##		At	LlegL
##		part	POWERS\CosmicPowers\Nictus\NictusCore1.part
##		part	POWERS\CosmicPowers\Nictus\NictusCoreEnergy2.part
##		part	POWERS\CosmicPowers\Nictus\NictusCoreEnergyTeal2.part
##		part	POWERS\CosmicPowers\Nictus\NictusEnergyPurple1.part
##		part	POWERS\CosmicPowers\Nictus\NictusEnergyTeal1.part
##		magnet	UlegL
##	End
##
##	Event
##		EName	Rleg
##		Type	Local
##		At	LlegR
##		part	POWERS\CosmicPowers\Nictus\NictusCore1.part
##		part	POWERS\CosmicPowers\Nictus\NictusCoreEnergy2.part
##		part	POWERS\CosmicPowers\Nictus\NictusCoreEnergyTeal2.part
##		part	POWERS\CosmicPowers\Nictus\NictusEnergyPurple1.part
##		part	POWERS\CosmicPowers\Nictus\NictusEnergyTeal1.part
##		magnet	UlegL
##	End
End

## PHYSICS FORCES ######################################################

Condition
	On	Time
	Time	5

	Event
		Ename	InitForce
		Type	Local
		At	Hips
		
		BhvrOverride
			PhysForceType		Out
			PhysForceRadius		8
			PhysForcePower		500
			PhysForcePowerJitter	50
		End

		Lifespan	10
	End
End

Condition
	On	Cycle
	Time	15

	Event
		Ename	ExpellingForce
		Type	Local
		At	Hips
		
		BhvrOverride
			PhysForceType		Out
			PhysForceRadius		6
			PhysForcePower		25
		End
		Lifespan	15
	End

#	Event
#		Ename	ExpellingForce
#		Type	Local
#		At	Hips
#		
#		BhvrOverride
#			PhysForceType		Up
#			PhysForceRadius		6
#			PhysForcePower		50
#		End
#		Lifespan	15
#	End
End

########################################################

End