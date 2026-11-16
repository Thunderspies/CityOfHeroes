#########################################################
##	FireAura
FxInfo

Lifespan	100
###############################################################################
###################################### Audio #####################################
###############################################################################

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	Origin
		Sound debtresist 88 88 .94
	End
End







#################################################################
############################# FX ########################################
#################################################################       #
									#
								       ###
Condition

	Event
		EName	wing
		Type	Local
		At	chest	
		
		Part	:Rays_Orange.part
		Part	:Rays_Green.part
		Part	:DebtResistance_Drips.part
		Part	:DebtResistance_Drips_Orange.part

		Bhvr	Behaviors/GenericParticleFade.bhvr
		Lifespan	70
				
	End

	Event
		EName	Hips2
		Type	Local
		At	Hips
		
		
		Part	:DebtResistance_Rays.part
		Part	:DebtResistance_Rays_Orange.part

		Bhvr	Behaviors/GenericParticleFade.bhvr
		Lifespan	70
	End

End
									#
Condition
	On 	time
	Time	5

	Event
		EName	headSparks
		Type	Local
		At	head
		Part4	:DebtResistance_Glow_Orange.part
		Part	:DebtResistance_Energy_Green.part
		
		Bhvr	Behaviors/GenericParticleFade.bhvr

	End

	Event
		EName	wing
		Type	Local
		At	chest	
		
		Part4	:DebtResistance_Glow_Orange.part
		Part	:DebtResistance_Energy_Green.part

		Bhvr	Behaviors/GenericParticleFade.bhvr
				
	End

	Event
		EName	Hips2
		Type	Local
		At	Hips
		
		Part4	:DebtResistance_Glow_Orange.part
		Part	:DebtResistance_Energy_Green.part

		Bhvr	Behaviors/GenericParticleFade.bhvr
	End
	
	Event
		EName	Lleg
		Type	Local
		At	UArmL
		Part4	:DebtResistance_Glow_Orange.part
		Part	:DebtResistance_Energy_Green.part
		
		Bhvr	Behaviors/GenericParticleFade.bhvr
	End

	Event
		EName	Rleg
		Type	Local
		At	UArmR
		Part4	:DebtResistance_Glow_Orange.part
		Part	:DebtResistance_Energy_Green.part
		
		Bhvr	Behaviors/GenericParticleFade.bhvr
	End

	Event
		EName	Lleg
		Type	Local
		At	LArmL
		Part4	:DebtResistance_Glow_Orange.part
		Part	:DebtResistance_Energy_Green.part
		
		Bhvr	Behaviors/GenericParticleFade.bhvr
	End

	Event
		EName	Rleg
		Type	Local
		At	LArmR
		Part4	:DebtResistance_Glow_Orange.part
		Part	:DebtResistance_Energy_Green.part
		
		Bhvr	Behaviors/GenericParticleFade.bhvr
	End

	Event
		EName	Lleg
		Type	Local
		At	UlegL
		Part4	:DebtResistance_Glow_Orange.part
		Part	:DebtResistance_Energy_Green.part
		
		Bhvr	Behaviors/GenericParticleFade.bhvr
	End

	Event
		EName	Rleg
		Type	Local
		At	UlegR
		Part4	:DebtResistance_Glow_Orange.part
		Part	:DebtResistance_Energy_Green.part
		
		Bhvr	Behaviors/GenericParticleFade.bhvr
	End

	Event
		EName	Lleg
		Type	Local
		At	LlegL
		Part4	:DebtResistance_Glow_Orange.part
		Part	:DebtResistance_Energy_Green.part
		
		Bhvr	Behaviors/GenericParticleFade.bhvr
	End

	Event
		EName	Rleg
		Type	Local
		At	LlegR
		Part4	:DebtResistance_Glow_Orange.part
		Part	:DebtResistance_Energy_Green.part
		
		Bhvr	Behaviors/GenericParticleFade.bhvr
	End

	Event
		EName	Lleg
		Type	Local
		At	FootL
		Part4	:DebtResistance_Glow_Orange.part
		Part	:DebtResistance_Energy_Green.part
		
		Bhvr	Behaviors/GenericParticleFade.bhvr
	End

	Event
		EName	Rleg
		Type	Local
		At	FootR
		Part4	:DebtResistance_Glow_Orange.part
		Part	:DebtResistance_Energy_Green.part
		
		Bhvr	Behaviors/GenericParticleFade.bhvr
	End

	Event
		EName	InitialSwordFire
		Type	Local 
		At	WepR
		Part4	:DebtResistance_Glow_Orange.part
		Part	:DebtResistance_Energy_Green.part
		
		Bhvr	Behaviors/GenericParticleFade.bhvr
	
	End


	Event
		EName	InitialSwordFire1
		Type	Local
		At	WepL
		Part4	:DebtResistance_Glow_Orange.part
		Part	:DebtResistance_Energy_Green.part
		
		Bhvr	Behaviors/GenericParticleFade.bhvr
	End

End

End
		