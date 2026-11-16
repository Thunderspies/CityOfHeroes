#########################################################
##	template

FxInfo

Lifespan 20

#########################################################
####################### AUDIO ###########################
#########################################################


Condition
	On	Time
	Time	0
	

	Event
		Type	Local
		At 	Origin
		Sound kinetic4 80 80 .84
	End
End


#########################################################
#################### BODY GLOW 1 ########################
#########################################################


Condition
	On	Time 
	Time	0
	
	Event	
		Type	Local
		At	Hips	
		Part	CustomizeablePowers\EnergyAura\KineticLightRaysBright.part
		
	End

	Event	
		ENAME	SpinNode
		Type	Local
		At	Hips	
		Bhvr	CustomizeablePowers\EnergyAura\KineticScale.bhvr
		
	End

	Event	
		ENAME	Swirl1
		Type	Local
		At	SpinNode
		part	CustomizeablePowers\EnergyAura\FaintKinecTestSwirl02.part
		part	CustomizeablePowers\EnergyAura\FaintKinecTestSwirl01.part
		Bhvr	CustomizeablePowers\EnergyAura\KineticSpin1.bhvr
		#Lifespan 20
		
	End

	Event	
		ENAME	Swirl2
		Type	Local
		At	SpinNode
		part	CustomizeablePowers\EnergyAura\FaintKinecTestSwirl02.part
		part	CustomizeablePowers\EnergyAura\FaintKinecTestSwirl01.part
		Bhvr	CustomizeablePowers\EnergyAura\KineticSpin2.bhvr
		#Lifespan 20
		
	End

	Event	
		ENAME	Swirl3
		Type	Local
		At	SpinNode
		part	CustomizeablePowers\EnergyAura\FaintKinecTestSwirl02.part
		part	CustomizeablePowers\EnergyAura\FaintKinecTestSwirl01.part
		Bhvr	CustomizeablePowers\EnergyAura\KineticSpin3.bhvr
		#Lifespan 20
		
	End

####################################################################################################
##Opposite Rings
################################################################################
	
	Event	
		ENAME	SpinNode2
		Type	Local
		At	Hips	
		Bhvr	CustomizeablePowers\EnergyAura\KineticScale.bhvr
		
	End

	Event	
		ENAME	Swirl1
		Type	Local
		At	SpinNode2
		part	CustomizeablePowers\EnergyAura\FaintKinecTestSwirl02.part
		part	CustomizeablePowers\EnergyAura\FaintKinecTestSwirl01.part
		Bhvr	CustomizeablePowers\EnergyAura\KineticSpin1.bhvr
		#Lifespan 20
		
	End

	Event	
		ENAME	Swirl2
		Type	Local
		At	SpinNode2
		part	CustomizeablePowers\EnergyAura\FaintKinecTestSwirl02.part
		part	CustomizeablePowers\EnergyAura\FaintKinecTestSwirl01.part
		Bhvr	CustomizeablePowers\EnergyAura\KineticSpin2.bhvr
		#Lifespan 20
		
	End

	Event	
		ENAME	Swirl3
		Type	Local
		At	SpinNode2
		part	CustomizeablePowers\EnergyAura\FaintKinecTestSwirl02.part
		part	CustomizeablePowers\EnergyAura\FaintKinecTestSwirl01.part
		Bhvr	CustomizeablePowers\EnergyAura\KineticSpin3.bhvr
		#Lifespan 20
		
	End

##############################################################################################

	Event	
		ENAME	Glows1
		Type	Local
		At	SpinNode
		part	CustomizeablePowers\EnergyAura\KineticExpandingRings01.part
		Bhvr	Behaviors\GenericParticleFade.bhvr
		#Lifespan 20
		
	End



End


End