#############################################################
## JoltingChain_Activation.fx
#############################################################

FxInfo
LifeSpan	80

#############################################################

Input
	Inpname	Hips
End

#############################################################


Condition
	On 	Time
	Time 	0

	Event
		EName 	Thingy
		Type	local
		At	HandR
		Sound EC_ParalyzingBlast_Activate_01 100 100 .8
		Part	:Hand_Electricity_Arcs_Consistant.part
		Part	:Hand_Electricity_Arcs_Consistant2.part
		Part	:Hand_Glow.part
		Part	:Hand_Ring.part
		Part	:Hand_Sparks.part
		POther	LArmR
	End

	Event
		EName 	Thingy1
		Type	local
		At	HandL
		Part	:Hand_Electricity_Arcs_Consistant.part
		Part	:Hand_Electricity_Arcs_Consistant2.part
		Part	:Hand_Glow.part
		Part	:Hand_Ring.part
		Part	:Hand_Sparks.part
	End

	Event
		Type	Local
		At	T_Root
		Part	:Root_Electricity_Glow_Flat_Gathering.part
		Part	:Root_Electricity_Glow_Flat_Stationary.part
		Lifespan 20
	End
End

Condition
	On	Time
	Time	30

	Event
		Type	Local
		At	T_Root
		Bhvr	behaviors/ScaleBubble2.bhvr
		BhvrOverride
			Alpha			150
			TintGeom		1
			Scale			1.0 1.0 1.0
			ScaleRate		5.0 5.0 5.0
 			EndScale		14 14 14
		End
		Geom	Tintable_ElectricityHit
		Lifespan 5
	End

	Event
		Type	Local
		At	T_Root
		BhvrOverride
			PositionOffset	0 .1 0
		End
		Part	:Root_Ring_Burst_Flat.part
		Part	:Hit_Shockwave.part
		Part	:Hit_Glow.part
		Part	:Hit_Glow_Flat.part
		Part	:Hit_Flash_Flat_Huge2.part
		Part	:Hit_Flash_Huge.part
		Part	:Hit_Flash_Huge2.part
		Part	:Hit_Electricity_Arcs.part
		Part	:Hit_Electricity_Arcs2.part
		Part	:Hit_Electricity_Flat_Arcs.part
		Lifespan 20
	End
End

Condition
	On	Time
	Time	34

	Event
		Type	Local
		At	T_Root
		Part	:Root_Ring_Burst.part
		Lifespan 3
	End
End

#############################################################

End