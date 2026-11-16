#############################################################
## Inferno.fx
#############################################################

FxInfo


#############################################################

Condition
	On	Time
	Time	0

	Event
		Ename	Anchor
		at	Root
		#type	local
		Type 	CREATE
  		Inherit	Position Rotation
  		Update	Position Rotation
	End
End


Condition
	On 	Cycle
	Time 	23

	Event
		EName   RadiusFxScale
		Type	local
		At	Anchor
		Bhvr	:HelicopterDust.bhvr
		Lifespan 150
	End


	Event
		EName 	Prime1
		Type	Local
		At	Anchor
		bhvr    :Dust_Rad.bhvr
	#	Part	:Helicopter_Dust_SM.part
	#	Part	:Helicopter_Dust_SM_LocalFacing.part
		Part	:Helicopter_DustRing_SM.part
		Lifespan 150
	End

	Event
		Ename	ExplosiveForce
		Type	Start
		At	RadiusFxScale

		BhvrOverride
			PhysForceType		Out
			PhysForceRadius		30
			PhysForcePower		400
			PhysForcePowerJitter	100
		End

		Lifespan	150

	End
End

#Condition
#	On 	Cycle
#	Time 	11
#
#
#	Event
#		EName   RadiusFxScale
#		Type	local
#		At	Anchor
#		Bhvr	:HelicopterDust.bhvr
#		Lifespan 150
#	End
#
#
#	Event
#		EName 	Prime1
#		Type	Local
#		At	RadiusFxScale
#		bhvr    :Dust_Rad.bhvr
#		BhvrOverride
#			PositionOffset 0 1 0
#		End
#	#	Part	:Helicopter_Dust_SM.part
#	#	Part	:Helicopter_Dust_SM_LocalFacing.part
#		Part	:Helicopter_DustRing_SM.part
#		Lifespan 150
#	End
#
#	Event
#		Ename	ExplosiveForce
#		Type	Start
#		At	RadiusFxScale
#
#		BhvrOverride
#			PhysForceType		Out
#			PhysForceRadius		30
#			PhysForcePower		400
#			PhysForcePowerJitter	100
#		End
#
#		Lifespan	150
#
#	End
#
#
#End


#############################################################

End