#############################################################
## Header
#############################################################

FxInfo

LifeSpan 200

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		ENAME   WeaponAnchor
		Type	CREATE
		At	Origin
  		Inherit	Rotation Position
  		Update	Rotation Position
	End
end

Condition
	On 	Time
	Time 	7

	Event
		ENAME   WeaponOffset
		Type	Local
		At	WeaponAnchor
		BhvrOverride
			PositionOffset 22 2 0
		End
		Geom	FlameThrower
		Sound Machgun_short 120.0 120.0 1.0
		Lifespan	14
	End

#	Event
#		Type	local
#		At	WeaponOffset
#		Altpiv	1
#		Part1	:TorchflameA.part
#		Part2	:TorchflameB.part
#		Lifespan	12
#	End
End

Condition
	On 	Time
	Time 	7

	Event
		Type	Local
		At	WeaponOffset
		Altpiv	1
		Bhvr	Behaviors/MachineGunBust.bhvr
		BhvrOverride
			Scale 5 5 5
		End
		Geom	FX_MachineGunBurst
		Part1	:MachineGunFlash.part
		Part2	:MachineGunGlow.part
		Part1	:TorchflameA.part
		Part2	:TorchflameB.part
		Lifespan	6
	End

#	Event
#		Type	Local
#		At	WepR
#		#ChildFX	V_COV\PhysicsEnabled\ARGunShellEject.fx
#		Lifespan	1
#	End
End

Condition
	On 	Time
	Time 	10

	Event
		Type	Local
		At	WeaponOffset
		Altpiv	1
		Bhvr	Behaviors/MachineGunBust.bhvr
		BhvrOverride
			PositionOffset 0 -2 4
			Scale 5 5 5
		End
		Geom	FX_MachineGunBurst
		Part1	:MachineGunFlash.part
		Part2	:MachineGunGlow.part
		Part1	:TorchflameA.part
		Part2	:TorchflameB.part
		Lifespan	6
	End

#	Event
#		Type	Local
#		At	WepR
#		#ChildFX	V_COV\PhysicsEnabled\ARGunShellEject.fx
#		Lifespan	1
#	End
End

Condition
	On 	Time
	Time 	13

	Event
		Type	Local
		At	WeaponOffset
		Altpiv	1
		Bhvr	Behaviors/MachineGunBust.bhvr
		BhvrOverride
			PositionOffset 0 -4 0
			Scale 5 5 5
		End
		Geom	FX_MachineGunBurst
		Part1	:MachineGunFlash.part
		Part2	:MachineGunGlow.part
		Part1	:TorchflameA.part
		Part2	:TorchflameB.part
		Lifespan	6
	End
#
#	Event
#		Type	Local
#		At	WepR
#		#ChildFX	V_COV\PhysicsEnabled\ARGunShellEject.fx
#		Lifespan	1
#	End
End


Condition
	On 	Time
	Time 	16

	Event
		Type	Local
		At	WeaponOffset
		Altpiv	1
		Bhvr	Behaviors/MachineGunBust.bhvr
		BhvrOverride
			PositionOffset 0 -2 -4
			Scale 5 5 5
		End
		Geom	FX_MachineGunBurst
		Part1	:MachineGunFlash.part
		Part2	:MachineGunGlow.part
		Part1	:TorchflameA.part
		Part2	:TorchflameB.part
		Lifespan	6
	End

#	Event
#		Type	Local
#		At	WepR
#		#ChildFX	V_COV\PhysicsEnabled\ARGunShellEject.fx
#		Lifespan	1
#	End
End

Condition
	On 	Time
	Time 	19

	Event
		Type	Local
		At	WeaponOffset
		Altpiv	1
		Bhvr	Behaviors/MachineGunBust.bhvr
		BhvrOverride
			Scale 5 5 5
		End
		Geom	FX_MachineGunBurst
		Part1	:MachineGunFlash.part
		Part2	:MachineGunGlow.part
		Part1	:TorchflameA.part
		Part2	:TorchflameB.part
		Lifespan	6
	End

#	Event
#		Type	Local
#		At	WepR
#		#ChildFX	V_COV\PhysicsEnabled\ARGunShellEject.fx
#		Lifespan	1
#	End
End

Condition
	On 	Time
	Time 	21

	Event
		Type	Local
		At	WeaponOffset
		Altpiv	1
		Bhvr	Behaviors/MachineGunBust.bhvr
		BhvrOverride
			PositionOffset 4 -2 0
			Scale 5 5 5
		End
		Geom	FX_MachineGunBurst
		Part1	:MachineGunFlash.part
		Part2	:MachineGunGlow.part
		Part1	:TorchflameA.part
		Part2	:TorchflameB.part
		Lifespan	6
	End

#	Event
#		Type	Local
#		At	WepR
#		#ChildFX	V_COV\PhysicsEnabled\ARGunShellEject.fx
#		Lifespan	1
#	End
End

Condition
	On 	Time
	Time 	24

	Event
		Type	Local
		At	WeaponOffset
		Altpiv	1
		Bhvr	Behaviors/MachineGunBust.bhvr
		BhvrOverride
			PositionOffset 0 -4 0
			Scale 5 5 5
		End
		Geom	FX_MachineGunBurst
		Part1	:MachineGunFlash.part
		Part2	:MachineGunGlow.part
		Part1	:TorchflameA.part
		Part2	:TorchflameB.part
		Lifespan	6
	End
#
#	Event
#		Type	Local
#		At	WepR
#		#ChildFX	V_COV\PhysicsEnabled\ARGunShellEject.fx
#		Lifespan	1
#	End
End


Condition
	On 	Time
	Time 	27

	Event
		Type	Local
		At	WeaponOffset
		Altpiv	1
		Bhvr	Behaviors/MachineGunBust.bhvr
		BhvrOverride
			PositionOffset -4 -2 0
			Scale 5 5 5
		End
		Geom	FX_MachineGunBurst
		Part1	:MachineGunFlash.part
		Part2	:MachineGunGlow.part
		Part1	:TorchflameA.part
		Part2	:TorchflameB.part
		Lifespan	6
	End

#	Event
#		Type	Local
#		At	WepR
#		#ChildFX	V_COV\PhysicsEnabled\ARGunShellEject.fx
#		Lifespan	1
#	End
End

#############################################################

End