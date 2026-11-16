#########################################################
##	RadiationAura
FxInfo

Input  
	InpName	T_root
End

Lifespan	25

##################################


Condition
	On	Time
	Time	0	

	Event
		EName	OverHeadNode
		Type	posit
		At	T_root
		Geom	OverHeadDummy01
		#Bhvr	Behaviors/GenericParticleFade.bhvr
		Sound cold_loop 100 100 .9
	End

End

Condition
	On	Time
	Time	0	

	Event
		EName	Snow01
		Type	local
		At	OverHeadNode
		part	POWERS\StormSummoning\SSSnow01.part
		#bhvr    behaviors/SSSpin4.bhvr
		Sound freezerain_loop 100 100 .22
		AltPiv	1
	End

#	Event
#		EName	Snow01
#		Type	local
#		At	OverHeadNode
#		part	POWERS\StormSummoning\SSSnow02.part
#		#bhvr    behaviors/SSSpin5.bhvr
#		AltPiv	1
#	End
#
End

Condition
	On	Time
	Time	20	

	Event
		EName	Snow01
		Type	posit
		At	T_root
		part	:SnowStormMist01.part
		bhvr    :Spin1.bhvr
		#Sound	cold_loop 100 30 .8
	End

End
#
#Condition
#	On	Time
#	Time	8	
#
#	Event
#		EName	Snow01
#		Type	posit
#		At	T_root
#		part	:SnowStormMist02.part
#		bhvr    behaviors/SSSpin2.bhvr
#	End
#
#	Event
#		EName	Snow01
#		Type	posit
#		At	T_root
#		part	:SnowStormMist03.part
#		bhvr    behaviors/SSSpin3.bhvr
#	End
#
#End
#

End		