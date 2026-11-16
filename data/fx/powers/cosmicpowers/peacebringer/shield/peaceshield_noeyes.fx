#########################################################
##	forceBubble
########################################################
FxInfo
	
#####################
##Audio
############################################

Condition
	On	Time	
	Time 	5

	Event
		Type 	Local
		At	Origin
		Sound glow6_loop 70.0 70.0 .44
		bhvr	behaviors/soundFade3.bhvr
		Flags	PowerLoopingSound
		Lifespan 220
	End

End

##########################################################
######################################################################
Condition
	On 	Time
	Time 	5

	Event
		Type	Posit
		At	HIPS
		Bhvr 	:PeaceShieldBHVR.bhvr
		
		Geom	CosmicPeaceBringerShield
				
	End

##ShieldCore##############################

	Event
		EName	Hips2
		Type	Local
		At	Hips
		
		part2	POWERS\CosmicPowers\PeaceBringer\PeaceCoreEnergy.part

		part2	POWERS\CosmicPowers\PeaceBringer\PeaceCore2.part
		
	End

	Event
		EName	Chest
		Type	Local
		At	Waist
		
		part2	POWERS\CosmicPowers\PeaceBringer\PeaceCore2.part
		
		Part4	POWERS\CosmicPowers\PeaceBringer\PeaceCoreSparks2.part
	End
	
	Event
		EName	Chest
		Type	Local
		At	Col_L
		part1	POWERS\CosmicPowers\PeaceBringer\PeaceCore2.part
		
	End

	Event
		EName	Chest
		Type	Local
		At	Col_R
		part1	POWERS\CosmicPowers\PeaceBringer\PeaceCore2.part
		
	End

	Event
		EName	Lleg
		Type	Local
		At	LlegL
		part1	POWERS\CosmicPowers\PeaceBringer\PeaceCore1.part
		part2	POWERS\CosmicPowers\PeaceBringer\PeaceCoreEnergy2.part
		magnet	UlegL
	End

	Event
		EName	Rleg
		Type	Local
		At	LlegR
		part1	POWERS\CosmicPowers\PeaceBringer\PeaceCore1.part
		part2	POWERS\CosmicPowers\PeaceBringer\PeaceCoreEnergy2.part
		magnet	UlegL
	End

	Event
		EName	Lleg
		Type	Local
		At	UlegL
		part1	POWERS\CosmicPowers\PeaceBringer\PeaceCore.part
		
	End

	Event
		EName	Rleg
		Type	Local
		At	UlegR
		part1	POWERS\CosmicPowers\PeaceBringer\PeaceCore.part
		
	End

	Event
		EName	Back
		Type	Local
		At	Back
		part1	POWERS\CosmicPowers\PeaceBringer\PeaceCore3.part
		
	End

End


Condition
	On	Time
	Time	15

	Event
		Ename	ExpellingForce
		Type	Local
		At	Hips
		
		BhvrOverride
			PhysForceType		Out
			PhysForceRadius		4
			PhysForcePower		25
		End
	End

	Event
		Ename	ForceField
		Type	Local
		At	Hips
		
		BhvrOverride
			PhysForceType		8
			PhysForceRadius		3.5
			PhysForcePower		2000
		End
	End

End



End
