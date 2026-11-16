#############################################################
## SSLSLightningStormCloudBolt.fx
#############################################################

FxInfo
LifeSpan	200

########################################################


######################################################################################

Condition

	On 	Time
	Time	0

	Event
		ENAME	Overhead
		Type	Posit
		At	Origin
		Geom	OverHeadDummy
		Sound lightningbolt4 120 120 .9
	End

End

Condition
	On 	Time
	Time 	18

	Event
		Type	start
		At	Overhead
		AltPiv	1
		Part	:ChargedBolt01.part
		Part	:ChargedBolt02.part
		POther Target
	End

End

Condition
	On 	Time
	Time 	12

	Event
		Type	start
		At	Overhead
		AltPiv	1
		Part	:SSLSLightningBoltAttackGlow01.part
		POther  Target
		LifeSpan 8
	End

End

######################################################################################


#############################################################################
##Hands
############################################################


Condition

	On 	Time
	Time	22

	Event
		ENAME	ThunderShake1
		Type	Local
		At	Root
		Bhvr	Behaviors/SSLSShake.bhvr
		LifeSpan 10
	End

End


Condition
	On 	PrimeHit

	Event
		EName	Prime
		Type	destroy
	End

End

Condition
	On 	Time
	Time 	65


	Event
		EName	ThingyB
		Type	destroy
	End



End

######################################################################################

End