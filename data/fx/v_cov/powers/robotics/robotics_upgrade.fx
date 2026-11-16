#########################################################
##	template

FxInfo

LifeSpan		300
#########################################################
####################### AUDIO ###########################
#########################################################



Condition
	On	Time
	Time	0
	

	Event
		Type	Local
		At 	T_root
		#Sound	Glow3_loop 80 30 .26
	End
End


#########################################################
#################### nanomites ########################
#########################################################

Condition
	On	Time
	Time	25
	
#	Event	
#		ENAME	Prime
#		Type	start
#		At	root
#		BHVR	:Projectile.Bhvr
#		#Geom	NanoMachine
#		
#		Magnet	T_Root
#		Lookat	T_Root
#	
#	End
#
#	Event	
#		Type	start
#		At	Root
#
#		ChildFx	V_COV\Powers\Robotics\Robotics_Nanomite.fx
#		LifeSpan	300
#	End
	
	Event	
		Type	local
		At	PrimeHit
		
		ChildFx	V_COV\Powers\Robotics/Robotics_Sparks.fx
		LifeSpan	300
	End

End

##########################################################
##################### four poles ########################
##########################################################
#
#Condition
#	On	Time
#	Time	35
#
#	Event	
#		ENAME	electricityUpgrade
#		Type	local
#		At	T_root
#		
#		ChildFx	:UpgradeElectricity.fx
#		#LifeSpan	150	
#	End
#	
#	Event	
#		ENAME	electricityUpgrade
#		Type	local
#		At	T_root
#		
#		ChildFx	:Robotics_Sparks_Long.fx
#		#LifeSpan	150	
#	End
#
#End
#
#Condition
#	On	Time
#	Time	15
#	
#	
#	Event	
#		ENAME	hookup
#		Type	local
#		At	T_root
#		
#		Geom	centerdummy
#			
#	End
#
#	Event	
#		Type	local
#		At	T_root
#		BHVR	:ScaffoldingScale.bhvr
#		Geom	FX_ScaffoldingPole
#		Part	:Electricity.part
#		Part	:Electricity2.part
#		Part	:ElectricitySparkUpa.part
#		Part	:ElectricitySparkUp2a.part
#		#LifeSpan	185	
#	End
#
#End
#
#Condition
#	On	Time
#	Time	23
#
#	Event	
#		Type	local
#		At	T_root
#		BHVR	:ScaffoldingScaleOpposit.bhvr
#		Geom	FX_ScaffoldingPole
#		Part	:Electricity.part
#		Part	:Electricity2.part
#		Part	:ElectricitySparkUpa.part
#		Part	:ElectricitySparkUp2a.part
#		#LifeSpan	177	
#			
#	End
#End	
#
#Condition
#	On	Time
#	Time	26
#
#	Event	
#		Type	local
#		At	T_root
#		BHVR	:ScaffoldingScalekittycorner.bhvr
#		Geom	FX_ScaffoldingPole
#		Part	:Electricity.part
#		Part	:Electricity2.part
#		Part	:ElectricitySparkUpa.part
#		Part	:ElectricitySparkUp2a.part
#		#LifeSpan	174	
#			
#	End
#End	
#
#Condition
#	On	Time
#	Time	29
#
#	Event	
#		Type	local
#		At	T_root
#		BHVR	:ScaffoldingScalekittycornerOpposite.bhvr
#		Geom	FX_ScaffoldingPole
#		Part	:Electricity.part
#		Part	:Electricity2.part
#		Part	:ElectricitySparkUpa.part
#		Part	:ElectricitySparkUp2a.part
#		#LifeSpan	171	
#			
#	End
#End	
#
#
##########################################################
##################### Horizontal poles ########################
##########################################################
#
#
#Condition
#	On	Time
#	Time	30
#	
#	Event	
#		Type	local
#		At	hookup
#		altpiv	1
#		BHVR	:ScaffoldingRotate.bhvr
#		Geom	FX_ScaffoldingPole02
#		#LifeSpan	170	
#			
#	End
#
#End
#
#Condition
#	On	Time
#	Time	33
#
#	Event	
#		Type	local
#		At	hookup
#		altpiv	1
#		BHVR	:ScaffoldingRotateOpposite.bhvr
#		Geom	FX_ScaffoldingPole02
#		#LifeSpan	167
#			
#	End
#End	
#
#Condition
#	On	Time
#	Time	38
#
#	Event	
#		Type	local
#		At	hookup
#		altpiv	1
#		BHVR	:ScaffoldingRotateUp.bhvr
#		Geom	FX_ScaffoldingPole02
#		#LifeSpan	162
#			
#	End
#End	
#
#Condition
#	On	Time
#	Time	36
#
#	Event	
#		Type	local
#		At	hookup
#		altpiv	1
#		BHVR	:ScaffoldingRotateUpOpposite.bhvr
#		Geom	FX_ScaffoldingPole02
#		#LifeSpan	165
#			
#	End
#End
#
#
##########################################################
##################### Horizontal poles Stage 2 ########################
##########################################################
#
#
#Condition
#	On	Time
#	Time	36
#	
#	Event	
#		Type	local
#		At	hookup
#		altpiv	1
#		BHVR	:ScaffoldingRotateStage2.bhvr
#		Geom	FX_ScaffoldingPole02
#		#LifeSpan	165
#			
#	End
#
#End
#
#Condition
#	On	Time
#	Time	40
#
#	Event	
#		Type	local
#		At	hookup
#		altpiv	1
#		BHVR	:ScaffoldingRotateOppositeStage2.bhvr
#		Geom	FX_ScaffoldingPole02
#		#LifeSpan	161
#			
#	End
#End	
#
#Condition
#	On	Time
#	Time	38
#
#	Event	
#		Type	local
#		At	hookup
#		altpiv	1
#		BHVR	:ScaffoldingRotateUpStage2.bhvr
#		Geom	FX_ScaffoldingPole02
#		#LifeSpan	163
#			
#	End
#End	
#
#Condition
#	On	Time
#	Time	42
#
#	Event	
#		Type	local
#		At	hookup
#		altpiv	1
#		BHVR	:ScaffoldingRotateUpOppositeStage2.bhvr
#		Geom	FX_ScaffoldingPole02
#		#LifeSpan	159
#			
#	End
#End
#
##############################################################################################################
###ScaffoldingDiagonal
##############################################################################################################
#
#Condition
#	On	Time
#	Time	42
#
#	Event	
#		Type	local
#		At	hookup
#		altpiv	1
#		BHVR	:ScaffoldingDiagonal.bhvr
#		Geom	FX_ScaffoldingPoleDiagonal
#		#LifeSpan	159
#			
#	End
#End
#
#Condition
#	On	Time
#	Time	44
#
#	Event	
#		Type	local
#		At	hookup
#		altpiv	1
#		BHVR	:ScaffoldingDiagonalOpposite.bhvr
#		Geom	FX_ScaffoldingPoleDiagonal
#		#LifeSpan	157
#			
#	End
#End
#
#Condition
#	On	Time
#	Time	48
#
#	Event	
#		Type	local
#		At	hookup
#		altpiv	1
#		BHVR	:ScaffoldingDiagonalUp.bhvr
#		Geom	FX_ScaffoldingPoleDiagonalOpposite
#		#LifeSpan	153
#			
#	End
#End
#
#Condition
#	On	Time
#	Time	54
#
#	Event	
#		Type	local
#		At	hookup
#		altpiv	1
#		BHVR	:ScaffoldingDiagonalOppositeUp.bhvr
#		Geom	FX_ScaffoldingPoleDiagonalOpposite
#		#LifeSpan	148
#			
#	End
#End
#
#
##########################################################
##################### Horizontal poles Stage 3 ########################
##########################################################
#
#
#Condition
#	On	Time
#	Time	54
#	
#	Event	
#		Type	local
#		At	hookup
#		altpiv	1
#		BHVR	:ScaffoldingRotateStage3.bhvr
#		Geom	FX_ScaffoldingPole02
#		#LifeSpan	148
#			
#	End
#
#End
#
#Condition
#	On	Time
#	Time	56
#
#	Event	
#		Type	local
#		At	hookup
#		altpiv	1
#		BHVR	:ScaffoldingRotateOppositeStage3.bhvr
#		Geom	FX_ScaffoldingPole02
#		#LifeSpan	146
#			
#	End
#End	
#
#Condition
#	On	Time
#	Time	61
#
#	Event	
#		Type	local
#		At	hookup
#		altpiv	1
#		BHVR	:ScaffoldingRotateUpStage3.bhvr
#		Geom	FX_ScaffoldingPole02
#		#LifeSpan	141
#			
#	End
#End	
#
#Condition
#	On	Time
#	Time	64
#
#	Event	
#		Type	local
#		At	hookup
#		altpiv	1
#		BHVR	:ScaffoldingRotateUpOppositeStage3.bhvr
#		Geom	FX_ScaffoldingPole02
#		#LifeSpan	138
#			
#	End
#End
#
#Condition
#	On	Time
#	Time	64
#
#	Event	
#		Type	local
#		At	hookup
#		altpiv	1
#		BHVR	:ScaffoldingRotateStage4.bhvr
#		Geom	FX_ScaffoldingPole02
#		#LifeSpan	138
#			
#	End
#End
#
#Condition
#	On	Time
#	Time	68
#
#	Event	
#		Type	local
#		At	hookup
#		altpiv	1
#		BHVR	:ScaffoldingRotateOppositeStage4.bhvr
#		Geom	FX_ScaffoldingPole02
#		#LifeSpan	134
#			
#	End
#End
#
#############################################################################################################

End