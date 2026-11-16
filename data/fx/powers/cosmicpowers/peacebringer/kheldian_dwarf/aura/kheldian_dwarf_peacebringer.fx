#############################################################
## FX System Name
#############################################################

FxInfo

Flags    InheritAlpha

#############################################################

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	Chest		
		Sound obsidian_loop 70 70 .13
		bhvr	behaviors/soundFade3.bhvr
		Flags	PowerLoopingSound
		Lifespan	220
	End
End


Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	Chest		
		sound	peaceeyes1 60 50 .7
	
	End
End

#############################################################

##Condition
##	On	Time
##	Time	0
##
##	Event
##		EName	Aura
##		Type	Local
##		At	Chest	
##		BHVR	Behaviors\GenericParticleFade.bhvr	
##		Part	:InfernorockBitsLingering.part		
##		LifeSpan	30
##	End
##End

#Condition
#	On	Time
#	Time	0
#
#	Event
#		EName	Aura
#		Type	Local
#		At	Chest		
#		bhvr	:PeaceAuraGlow_Eye_R.bhvr
#		part	:PeaceAuraGlow_Eye_R.part
#	End
#
#	Event
#		EName	Aura
#		Type	Local
#		At	Chest		
#		bhvr	:PeaceAuraGlow_Eye_L.bhvr
#		part	:PeaceAuraGlow_Eye_R.part
#	End
#End

Condition
	On	Time
	Time	0
	
	Event
		Type	Local
		At	Chest	
		BhvrOverride
			PositionOffset	0.0 1.1 -0.7
		End
		part2	:Aura_EnergyTendrils_Medium.part
	End

	Event
		Type	Local
		At	UarmR		
		part2	:Aura_EnergyTendrils_Medium.part
	End

	Event
		Type	Local
		At	UarmL		
		part2	:Aura_EnergyTendrils_Medium.part
	End

	Event
		Type	Local
		At	LarmR		
		part2	:Aura_EnergyTendrils_Medium.part
	End

	Event
		Type	Local
		At	LarmL		
		part2	:Aura_EnergyTendrils_Medium.part
	End

	Event
		Type	Local
		At	HandR		
		part2	:Aura_EnergyTendrils_Medium.part
	End

	Event
		Type	Local
		At	HandL		
		part2	:Aura_EnergyTendrils_Medium.part
	End

	Event
		Type	Local
		At	ULegR		
		part2	:Aura_EnergyTendrils_Small.part
	End

	Event
		Type	Local
		At	ULegL		
		BhvrOverride
			PositionOffset		0.0 -0.6 0.0
		End
		part2	:Aura_EnergyTendrils_Small.part
	End

	Event
		Type	Local
		At	LLegR		
		part2	:Aura_EnergyTendrils_Small.part
	End

	Event
		Type	Local
		At	LLegL		
		part2	:Aura_EnergyTendrils_Small.part
	End

	Event
		Type	Local
		At	FootR		
		part2	:Aura_EnergyTendrils_Small.part
	End

	Event
		Type	Local
		At	FootL		
		part2	:Aura_EnergyTendrils_Small.part
	End
End

#############################################################

End				