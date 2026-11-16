#########################################################
##	template

FxInfo

Input
	InpName Origin
End

Input  
	InpName	Target
End

Input  
	InpName	hips
End

Input  
	InpName	Eyes
End




#########################################################
####################### AUDIO ###########################
#########################################################




#########################################################
#################### BODY GLOW 1 ########################
#########################################################

Condition


	Event	
		ENAME	Chestnode
		Type	Local
		At	Chest
		At	chest
		Bhvr	Behaviors/Spin.bhvr
		Geom	ChestPivotsBuffToHit
		LifeSpan	60
	End

	Event	
		ENAME	Front
		Type	Local
		At	Chestnode
		AltPiv	2
		part	:BuffDamResShield.part
		part	:BuffDamResChevron01.part
		part	:BuffDamResChevron03.part
		part	:BuffDamResChevron02.part
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Lifespan 60
	
	End

	Event	
		ENAME	Back
		Type	Local
		At	Chestnode
		AltPiv	3
		part	:BuffDamResShield.part
		part	:BuffDamResChevron01.part
		part	:BuffDamResChevron03.part
		part	:BuffDamResChevron02.part
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Lifespan 60
	
	End

	Event	
		ENAME	Front
		Type	Local
		At	Chestnode
		AltPiv	5
		part	:BuffDamResShield.part
		part	:BuffDamResChevron01.part
		part	:BuffDamResChevron03.part
		part	:BuffDamResChevron02.part
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Lifespan 60
	
	End

	Event	
		ENAME	Back
		Type	Local
		At	Chestnode
		AltPiv	6
		part	:BuffDamResShield.part
		part	:BuffDamResChevron01.part
		part	:BuffDamResChevron03.part
		part	:BuffDamResChevron02.part
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Lifespan 60
	
	End

End

Condition
	On	Cycle		
	Time	100

	Event	
		ENAME	Chestnode
		Type	Local
		At	Chest
		At	chest
		Bhvr	Behaviors/Spin.bhvr
		Geom	ChestPivotsBuffToHit
		LifeSpan	60
	End

	Event	
		ENAME	Front
		Type	Local
		At	Chestnode
		AltPiv	2
		part	:BuffDamResShield.part
		part	:BuffDamResChevron01.part
		part	:BuffDamResChevron03.part
		part	:BuffDamResChevron02.part
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Lifespan 60
	
	End

	Event	
		ENAME	Back
		Type	Local
		At	Chestnode
		AltPiv	3
		part	:BuffDamResShield.part
		part	:BuffDamResChevron01.part
		part	:BuffDamResChevron03.part
		part	:BuffDamResChevron02.part
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Lifespan 60
	
	End

	Event	
		ENAME	Front
		Type	Local
		At	Chestnode
		AltPiv	5
		part	:BuffDamResShield.part
		part	:BuffDamResChevron01.part
		part	:BuffDamResChevron03.part
		part	:BuffDamResChevron02.part
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Lifespan 60
	
	End

	Event	
		ENAME	Back
		Type	Local
		At	Chestnode
		AltPiv	6
		part	:BuffDamResShield.part
		part	:BuffDamResChevron01.part
		part	:BuffDamResChevron03.part
		part	:BuffDamResChevron02.part
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Lifespan 60
	
	End

End	


End