#########################################################
##	template

FxInfo

LifeSpan		885
#########################################################
####################### AUDIO ###########################
#########################################################



Condition
	On	Time
	Time	0
	

	Event
		Type	Local
		At 	Root
		#Sound	Glow3_loop 80 30 .26
	End
End

###################################################################################
##Electricity Action
#########################################################################3

Condition
	On	Time
	Time	10
	
	
	Event	
		ENAME	electricityhookup
		Type	local
		At	Root
		
		ChildFx	:ScaffoldingLights.fx

		LifeSpan	285
	End

End

#########################################################
#################### four poles ########################
#########################################################

Condition
	On	Time
	Time	0

	Event	
		ENAME	electricityUpgrade
		Type	local
		At	Root
		
		ChildFx	:UpgradeElectricity.fx
		LifeSpan	285	
	End
	
	Event	
		ENAME	electricityUpgrade
		Type	local
		At	Root
		
		ChildFx	:Robotics_Sparks_Long.fx
		LifeSpan	285	
	End

End

Condition
	On	Time
	Time	0
	
	
	Event	
		ENAME	hookup
		Type	local
		At	Root
		
		Geom	centerdummy
		LifeSpan		285	
	End

	Event	
		Type	local
		At	Root
		BHVR	:ScaffoldingScale.bhvr
		Geom	FX_ScaffoldingPole
		Part	:Electricity.part
		Part	:Electricity2.part
		Part	:ElectricitySparkUpa.part
		Part	:ElectricitySparkUp2a.part
		LifeSpan		275	
	End

End

Condition
	On	Time
	Time	8

	Event	
		Type	local
		At	Root
		BHVR	:ScaffoldingScaleOpposit.bhvr
		Geom	FX_ScaffoldingPole
		Part	:Electricity.part
		Part	:Electricity2.part
		Part	:ElectricitySparkUpa.part
		Part	:ElectricitySparkUp2a.part
		LifeSpan		275	
			
	End
End	

Condition
	On	Time
	Time	11

	Event	
		Type	local
		At	Root
		BHVR	:ScaffoldingScalekittycorner.bhvr
		Geom	FX_ScaffoldingPole
		Part	:Electricity.part
		Part	:Electricity2.part
		Part	:ElectricitySparkUpa.part
		Part	:ElectricitySparkUp2a.part
		LifeSpan		275	
			
	End
End	

Condition
	On	Time
	Time	14

	Event	
		Type	local
		At	Root
		BHVR	:ScaffoldingScalekittycornerOpposite.bhvr
		Geom	FX_ScaffoldingPole
		Part	:Electricity.part
		Part	:Electricity2.part
		Part	:ElectricitySparkUpa.part
		Part	:ElectricitySparkUp2a.part
		LifeSpan		275	
			
	End
End	


#########################################################
#################### Horizontal poles ########################
#########################################################


Condition
	On	Time
	Time	15
	
	Event	
		Type	local
		At	hookup
		altpiv	1
		BHVR	:ScaffoldingRotate.bhvr
		Geom	FX_ScaffoldingPole02
		LifeSpan		285
			
	End

End

Condition
	On	Time
	Time	28

	Event	
		Type	local
		At	hookup
		altpiv	1
		BHVR	:ScaffoldingRotateOpposite.bhvr
		Geom	FX_ScaffoldingPole02
		LifeSpan		285
			
	End
End	

Condition
	On	Time
	Time	23

	Event	
		Type	local
		At	hookup
		altpiv	1
		BHVR	:ScaffoldingRotateUp.bhvr
		Geom	FX_ScaffoldingPole02
		LifeSpan		285
			
	End
End	

Condition
	On	Time
	Time	21

	Event	
		Type	local
		At	hookup
		altpiv	1
		BHVR	:ScaffoldingRotateUpOpposite.bhvr
		Geom	FX_ScaffoldingPole02
		LifeSpan		285
			
	End
End


#########################################################
#################### Horizontal poles Stage 2 ########################
#########################################################


Condition
	On	Time
	Time	21
	
	Event	
		Type	local
		At	hookup
		altpiv	1
		BHVR	:ScaffoldingRotateStage2.bhvr
		Geom	FX_ScaffoldingPole02
		LifeSpan		285
			
	End

End

Condition
	On	Time
	Time	25

	Event	
		Type	local
		At	hookup
		altpiv	1
		BHVR	:ScaffoldingRotateOppositeStage2.bhvr
		Geom	FX_ScaffoldingPole02
		LifeSpan		285
			
	End
End	

Condition
	On	Time
	Time	23

	Event	
		Type	local
		At	hookup
		altpiv	1
		BHVR	:ScaffoldingRotateUpStage2.bhvr
		Geom	FX_ScaffoldingPole02
		LifeSpan		285
			
	End
End	

Condition
	On	Time
	Time	27

	Event	
		Type	local
		At	hookup
		altpiv	1
		BHVR	:ScaffoldingRotateUpOppositeStage2.bhvr
		Geom	FX_ScaffoldingPole02
		LifeSpan		285
			
	End
End

#############################################################################################################
##ScaffoldingDiagonal
#############################################################################################################

Condition
	On	Time
	Time	27

	Event	
		Type	local
		At	hookup
		altpiv	1
		BHVR	:ScaffoldingDiagonal.bhvr
		Geom	FX_ScaffoldingPoleDiagonal
		LifeSpan		285
			
	End
End

Condition
	On	Time
	Time	29

	Event	
		Type	local
		At	hookup
		altpiv	1
		BHVR	:ScaffoldingDiagonalOpposite.bhvr
		Geom	FX_ScaffoldingPoleDiagonal
		LifeSpan		285
			
	End
End

#######################################################
##Nonomite return
###################################################################################


Condition
	On	Time
	Time	290

	Event	
		ENAME	EmptyTimeNode
		Type	start
		At	root
		
		Bhvr	Behaviors/GenericParticleFade.bhvr
		ChildFx		:Robotics_Nanomite.fx

		LifeSpan	800

	End

End

	
End
