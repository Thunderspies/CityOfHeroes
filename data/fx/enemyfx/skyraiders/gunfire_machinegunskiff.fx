#########################################################
##	
##

FxInfo
LifeSpan  220

################################################################################################
Condition
	On 	Time
	Time 	1

	Event
		ENAME   t2
		Type	Local 
		At	mystic
		
		Part1	:Gunfire_AssaultShell2.Part
		Bhvr	Behaviors/MachineGunBustOffset.bhvr
		Sound gatgun3 120.0 120.0 .9
	End
	
	Event
		ENAME   t2a
		Type	Local 
		At	mystic
		
		Part1	:Gunfire_AssaultShell2.Part
		Bhvr	Behaviors/MachineGunBustOffsetL.bhvr

	End

End

Condition
	On 	Time
	Time 	0

	Event
		EName 	Blast1
		Type	local
		At	mystic
		Geom	FX_MachineGunBurst
		Bhvr	Behaviors/MachineGunBustOffset.bhvr
		Part1	:MachineGunFlash.part
		Part2	:MachineGunGlow.part
	End

	Event
		EName 	tracer
		Type	local
		At	mystic
		Bhvr	Behaviors/MachineGunBustOffset.bhvr
		Part1	:TorchflameA.part
		Part2	:TorchflameB.part
		
	End

	Event
		EName 	Blast1a
		Type	local
		At	mystic
		Geom	FX_MachineGunBurst
		Bhvr	Behaviors/MachineGunBustOffsetL.bhvr
		Part1	:MachineGunFlash.part
		Part2	:MachineGunGlow.part
	End

	Event
		EName 	tracera
		Type	local
		At	mystic
		Bhvr	Behaviors/MachineGunBustOffsetL.bhvr
		Part1	:TorchflameA.part
		Part2	:TorchflameB.part
		
	End

End

Condition
	On 	Time
	Time	46

	Event
		Ename 	tracer
		Type	Destroy
	End
	
	Event
		Ename 	tracera
		Type	Destroy
	End

End

Condition
	On 	Time
	Time	45

	Event
		Ename 	T2
		Type	Destroy
	End
	
	Event
		Ename 	T2a
		Type	Destroy
	End
End

Condition
	On 	Time
	Time 	3

	Event
		EName 	Blast2
		Type	local
		At	mystic
		Geom	FX_MachineGunBurst
		Bhvr	Behaviors/MachineGunBustOffset.bhvr
		Part1	:MachineGunFlash.part
		Part2	:MachineGunGlow.part
	End

	Event
		EName 	Blast2a
		Type	local
		At	mystic
		Geom	FX_MachineGunBurst
		Bhvr	Behaviors/MachineGunBustOffsetL.bhvr
		Part1	:MachineGunFlash.part
		Part2	:MachineGunGlow.part
	End

End


Condition
	On 	Time
	Time 	6

	Event
		EName 	Blast3
		Type	local
		At	mystic
		Geom	FX_MachineGunBurst
		Bhvr	Behaviors/MachineGunBustOffset.bhvr
		Part1	:MachineGunFlash.part
		Part2	:MachineGunGlow.part
	End

	Event
		EName 	Blast3a
		Type	local
		At	mystic
		Geom	FX_MachineGunBurst
		Bhvr	Behaviors/MachineGunBustOffsetL.bhvr
		Part1	:MachineGunFlash.part
		Part2	:MachineGunGlow.part
	End
End

Condition
	On 	Time
	Time 	9

	Event
		EName 	Blast4
		Type	local
		At	mystic
		Geom	FX_MachineGunBurst
		Bhvr	Behaviors/MachineGunBustOffset.bhvr
		Part1	:MachineGunFlash.part
		Part2	:MachineGunGlow.part
	End

	Event
		EName 	Blast4a
		Type	local
		At	mystic
		Geom	FX_MachineGunBurst
		Bhvr	Behaviors/MachineGunBustOffsetL.bhvr
		Part1	:MachineGunFlash.part
		Part2	:MachineGunGlow.part
	End

End

Condition
	On 	Time
	Time 	12

	Event
		EName 	Blast5
		Type	local
		At	mystic
		Geom	FX_MachineGunBurst
		Bhvr	Behaviors/MachineGunBustOffset.bhvr
		Part1	:MachineGunFlash.part
		Part2	:MachineGunGlow.part
	End

	Event
		EName 	Blast5a
		Type	local
		At	mystic
		Geom	FX_MachineGunBurst
		Bhvr	Behaviors/MachineGunBustOffsetL.bhvr
		Part1	:MachineGunFlash.part
		Part2	:MachineGunGlow.part
	End

End

Condition
	On 	Time
	Time 	15

	Event
		EName 	Blast6
		Type	local
		At	mystic
		Geom	FX_MachineGunBurst
		Bhvr	Behaviors/MachineGunBustOffset.bhvr
		Part1	:MachineGunFlash.part
		Part2	:MachineGunGlow.part
	End

	Event
		EName 	Blast6a
		Type	local
		At	mystic
		Geom	FX_MachineGunBurst
		Bhvr	Behaviors/MachineGunBustOffsetL.bhvr
		Part1	:MachineGunFlash.part
		Part2	:MachineGunGlow.part
	End

End

Condition
	On 	Time
	Time 	18

	Event
		EName 	Blast7
		Type	local
		At	mystic
		Geom	FX_MachineGunBurst
		Bhvr	Behaviors/MachineGunBustOffset.bhvr
		Part1	:MachineGunFlash.part
		Part2	:MachineGunGlow.part
	End

	Event
		EName 	Blast7a
		Type	local
		At	mystic
		Geom	FX_MachineGunBurst
		Bhvr	Behaviors/MachineGunBustOffsetL.bhvr
		Part1	:MachineGunFlash.part
		Part2	:MachineGunGlow.part
	End

End

Condition
	On 	Time
	Time 	21

	Event
		EName 	Blast8
		Type	local
		At	mystic
		Geom	FX_MachineGunBurst
		Bhvr	Behaviors/MachineGunBustOffset.bhvr
		Part1	:MachineGunFlash.part
		Part2	:MachineGunGlow.part
	End

	Event
		EName 	Blast8a
		Type	local
		At	mystic
		Geom	FX_MachineGunBurst
		Bhvr	Behaviors/MachineGunBustOffsetL.bhvr
		Part1	:MachineGunFlash.part
		Part2	:MachineGunGlow.part
	End

End

Condition
	On 	Time
	Time 	24

	Event
		EName 	Blast9
		Type	local
		At	mystic
		Geom	FX_MachineGunBurst
		Bhvr	Behaviors/MachineGunBustOffset.bhvr
		Part1	:MachineGunFlash.part
		Part2	:MachineGunGlow.part
	End

	Event
		EName 	Blast9a
		Type	local
		At	mystic
		Geom	FX_MachineGunBurst
		Bhvr	Behaviors/MachineGunBustOffsetL.bhvr
		Part1	:MachineGunFlash.part
		Part2	:MachineGunGlow.part
	End

End

Condition
	On 	Time
	Time 	27

	Event
		EName 	Blast10
		Type	local
		At	mystic
		Geom	FX_MachineGunBurst
		Bhvr	Behaviors/MachineGunBustOffset.bhvr
		Part1	:MachineGunFlash.part
		Part2	:MachineGunGlow.part
	End

	Event
		EName 	Blast10a
		Type	local
		At	mystic
		Geom	FX_MachineGunBurst
		Bhvr	Behaviors/MachineGunBustOffsetL.bhvr
		Part1	:MachineGunFlash.part
		Part2	:MachineGunGlow.part
	End


End

Condition
	On 	Time
	Time 	30

	Event
		EName 	Blast11
		Type	local
		At	mystic
		Geom	FX_MachineGunBurst
		Bhvr	Behaviors/MachineGunBustOffset.bhvr
		Part1	:MachineGunFlash.part
		Part2	:MachineGunGlow.part
	End

	Event
		EName 	Blast11a
		Type	local
		At	mystic
		Geom	FX_MachineGunBurst
		Bhvr	Behaviors/MachineGunBustOffsetL.bhvr
		Part1	:MachineGunFlash.part
		Part2	:MachineGunGlow.part
	End

End

Condition
	On 	Time
	Time 	33

	Event
		EName 	Blast12
		Type	local
		At	mystic
		Geom	FX_MachineGunBurst
		Bhvr	Behaviors/MachineGunBustOffset.bhvr
		Part1	:MachineGunFlash.part
		Part2	:MachineGunGlow.part
	End

	Event
		EName 	Blast12a
		Type	local
		At	mystic
		Geom	FX_MachineGunBurst
		Bhvr	Behaviors/MachineGunBustOffsetL.bhvr
		Part1	:MachineGunFlash.part
		Part2	:MachineGunGlow.part
	End

End

Condition
	On 	Time
	Time 	36

	Event
		EName 	Blast13
		Type	local
		At	mystic
		Geom	FX_MachineGunBurst
		Bhvr	Behaviors/MachineGunBustOffset.bhvr
		Part1	:MachineGunFlash.part
		Part2	:MachineGunGlow.part
	End

	Event
		EName 	Blast13a
		Type	local
		At	mystic
		Geom	FX_MachineGunBurst
		Bhvr	Behaviors/MachineGunBustOffsetL.bhvr
		Part1	:MachineGunFlash.part
		Part2	:MachineGunGlow.part
	End

End

Condition
	On 	Time
	Time 	39

	Event
		EName 	Blast14
		Type	local
		At	mystic
		Geom	FX_MachineGunBurst
		Bhvr	Behaviors/MachineGunBustOffset.bhvr
		Part1	:MachineGunFlash.part
		Part2	:MachineGunGlow.part
	End

	Event
		EName 	Blast14a
		Type	local
		At	mystic
		Geom	FX_MachineGunBurst
		Bhvr	Behaviors/MachineGunBustOffsetL.bhvr
		Part1	:MachineGunFlash.part
		Part2	:MachineGunGlow.part
	End

End


Condition
	On 	Time
	Time 	42

	Event
		EName 	Blast15
		Type	local
		At	mystic
		Geom	FX_MachineGunBurst
		Bhvr	Behaviors/MachineGunBustOffset.bhvr
		Part1	:MachineGunFlash.part
		Part2	:MachineGunGlow.part
	End

	Event
		EName 	Blast15a
		Type	local
		At	mystic
		Geom	FX_MachineGunBurst
		Bhvr	Behaviors/MachineGunBustOffsetL.bhvr
		Part1	:MachineGunFlash.part
		Part2	:MachineGunGlow.part
	End

End

Condition
	On 	Time
	Time 	45

	Event
		EName 	Blast16
		Type	local
		At	mystic
		Geom	FX_MachineGunBurst
		Bhvr	Behaviors/MachineGunBustOffset.bhvr
		
		
		LifeSpan	3
	End

#	Event
#		EName 	Blast16b
#		Type	local
#		At	mystic
#		Part1	:MachineGunFlash.part
#		Part2	:MachineGunGlow.part
#		
#		LifeSpan	1
#	End

	Event
		EName 	Blast16a
		Type	local
		At	mystic
		Geom	FX_MachineGunBurst
		Bhvr	Behaviors/MachineGunBustOffset.bhvr
		
		
		LifeSpan	3
	End

#	Event
#		EName 	Blast16ab
#		Type	local
#		At	mystic
#		Part1	:MachineGunFlash.part
#		Part2	:MachineGunGlow.part
#		LifeSpan	1
#	End

End

#Condition
#	On 	Time
#	Time 	51
#
#	Event
#		EName 	Blast17
#		Type	local
#		At	mystic
#		Geom	FX_MachineGunBurst
#		Bhvr	Behaviors/MachineGunBustOffset.bhvr
#		Part1	:MachineGunFlash.part
#		Part2	:MachineGunGlow.part
#	End
#
#	Event
#		EName 	Blast17a
#		Type	local
#		At	mystic
#		Geom	FX_MachineGunBurst
#		Bhvr	Behaviors/MachineGunBustOffsetL.bhvr
#		Part1	:MachineGunFlash.part
#		Part2	:MachineGunGlow.part
#	End
#
#End
#
#Condition
#	On 	Time
#	Time 	54
#
#	Event
#		EName 	Blast18
#		Type	local
#		At	mystic
#		Geom	FX_MachineGunBurst
#		Bhvr	Behaviors/MachineGunBustOffset.bhvr
#		Part1	:MachineGunFlash.part
#		Part2	:MachineGunGlow.part
#	End
#
#
#	Event
#		EName 	Blast18a
#		Type	local
#		At	mystic
#		Geom	FX_MachineGunBurst
#		Bhvr	Behaviors/MachineGunBustOffsetL.bhvr
#		Part1	:MachineGunFlash.part
#		Part2	:MachineGunGlow.part
#	End
#End
#
#Condition
#	On 	Time
#	Time 	57
#
#	Event
#		EName 	Blast19
#		Type	local
#		At	mystic
#		Geom	FX_MachineGunBurst
#		Bhvr	Behaviors/MachineGunBustOffset.bhvr
#		Part1	:MachineGunFlash.part
#		Part2	:MachineGunGlow.part
#	End
#
#	Event
#		EName 	Blast19a
#		Type	local
#		At	mystic
#		Geom	FX_MachineGunBurst
#		Bhvr	Behaviors/MachineGunBustOffsetL.bhvr
#		Part1	:MachineGunFlash.part
#		Part2	:MachineGunGlow.part
#	End
#End
#
#
#Condition
#	On 	Time
#	Time 	60
#
#	Event
#		EName 	Blast20
#		Type	local
#		At	mystic
#		Geom	FX_MachineGunBurst
#		Bhvr	Behaviors/MachineGunBustOffset.bhvr
#		Part1	:MachineGunFlash.part
#		Part2	:MachineGunGlow.part
#	End
#
#	Event
#		EName 	Blast20a
#		Type	local
#		At	mystic
#		Geom	FX_MachineGunBurst
#		Bhvr	Behaviors/MachineGunBustOffsetL.bhvr
#		Part1	:MachineGunFlash.part
#		Part2	:MachineGunGlow.part
#	End
#
#End
#
#Condition
#	On 	Time
#	Time 	63
#
#	Event
#		EName 	Blast21
#		Type	local
#		At	mystic
#		Geom	FX_MachineGunBurst
#		Bhvr	Behaviors/MachineGunBustOffset.bhvr
#		Part1	:MachineGunFlash.part
#		Part2	:MachineGunGlow.part
#	End
#
#	Event
#		EName 	Blast21a
#		Type	local
#		At	mystic
#		Geom	FX_MachineGunBurst
#		Bhvr	Behaviors/MachineGunBustOffsetL.bhvr
#		Part1	:MachineGunFlash.part
#		Part2	:MachineGunGlow.part
#	End
#
#End
#
#Condition
#	On 	Time
#	Time 	66
#
#	Event
#		EName 	Blast22
#		Type	local
#		At	mystic
#		Geom	FX_MachineGunBurst
#		Bhvr	Behaviors/MachineGunBustOffset.bhvr
#		Part1	:MachineGunFlash.part
#		Part2	:MachineGunGlow.part
#	End
#
#	Event
#		EName 	Blast22a
#		Type	local
#		At	mystic
#		Geom	FX_MachineGunBurst
#		Bhvr	Behaviors/MachineGunBustOffsetL.bhvr
#		Part1	:MachineGunFlash.part
#		Part2	:MachineGunGlow.part
#	End
#
#End
#
#Condition
#	On 	Time
#	Time 	69
#
#	Event
#		EName 	Blast23
#		Type	local
#		At	mystic
#		Geom	FX_MachineGunBurst
#		Bhvr	Behaviors/MachineGunBustOffset.bhvr
#		Part1	:MachineGunFlash.part
#		Part2	:MachineGunGlow.part
#	End
#
#	Event
#		EName 	Blast23a
#		Type	local
#		At	mystic
#		Geom	FX_MachineGunBurst
#		Bhvr	Behaviors/MachineGunBustOffsetL.bhvr
#		Part1	:MachineGunFlash.part
#		Part2	:MachineGunGlow.part
#	End
#
#End 
#
#Condition
#	On 	Time
#	Time 	72
#
#	Event
#		EName 	Blast24
#		Type	local
#		At	mystic
#		Geom	FX_MachineGunBurst
#		Bhvr	Behaviors/MachineGunBustOffset.bhvr
#		Part1	:MachineGunFlash.part
#		Part2	:MachineGunGlow.part
#	End
#
#	Event
#		EName 	Blast24a
#		Type	local
#		At	mystic
#		Geom	FX_MachineGunBurst
#		Bhvr	Behaviors/MachineGunBustOffsetL.bhvr
#		Part1	:MachineGunFlash.part
#		Part2	:MachineGunGlow.part
#	End
#
#End
#
#Condition
#	On 	Time
#	Time 	75
#
#	Event
#		EName 	Blast25
#		Type	local
#		At	mystic
#		Geom	FX_MachineGunBurst
#		Bhvr	Behaviors/MachineGunBustOffset.bhvr
#		Part1	:MachineGunFlash.part
#		Part2	:MachineGunGlow.part
#	End
# 
#
#	Event
#		EName 	Blast25a
#		Type	local
#		At	mystic
#		Geom	FX_MachineGunBurst
#		Bhvr	Behaviors/MachineGunBustOffsetL.bhvr
#		Part1	:MachineGunFlash.part
#		Part2	:MachineGunGlow.part
#	End
#End
#
#Condition
#	On 	Time
#	Time 	78
#
#	Event
#		EName 	Blast26
#		Type	local
#		At	mystic
#		Geom	FX_MachineGunBurst
#		Bhvr	Behaviors/MachineGunBustOffset.bhvr
#		Part1	:MachineGunFlash.part
#		Part2	:MachineGunGlow.part
#	End
#
#	Event
#		EName 	Blast26a
#		Type	local
#		At	mystic
#		Geom	FX_MachineGunBurst
#		Bhvr	Behaviors/MachineGunBustOffsetL.bhvr
#		Part1	:MachineGunFlash.part
#		Part2	:MachineGunGlow.part
#	End
#
#End
#
#Condition
#	On 	Time
#	Time 	81
#
#	Event
#		EName 	Blast27
#		Type	local
#		At	mystic
#		Geom	FX_MachineGunBurst
#		Bhvr	Behaviors/MachineGunBustOffset.bhvr
#		Part1	:MachineGunFlash.part
#		Part2	:MachineGunGlow.part
#	End
#
#	Event
#		EName 	Blast27a
#		Type	local
#		At	mystic
#		Geom	FX_MachineGunBurst
#		Bhvr	Behaviors/MachineGunBustOffsetL.bhvr
#		Part1	:MachineGunFlash.part
#		Part2	:MachineGunGlow.part
#	End
#
#End
#
#Condition
#	On 	Time
#	Time 	84
#
#	Event
#		EName 	Blast28
#		Type	local
#		At	mystic
#		Geom	FX_MachineGunBurst
#		Bhvr	Behaviors/MachineGunBustOffset.bhvr
#		Part1	:MachineGunFlash.part
#		Part2	:MachineGunGlow.part
#	End
#
#	Event
#		EName 	Blast28a
#		Type	local
#		At	mystic
#		Geom	FX_MachineGunBurst
#		Bhvr	Behaviors/MachineGunBustOffsetL.bhvr
#		Part1	:MachineGunFlash.part
#		Part2	:MachineGunGlow.part
#	End
#
#End
#
#Condition
#	On 	Time
#	Time 	87
#
#	Event
#		EName 	Blast29
#		Type	local
#		At	mystic
#		Geom	FX_MachineGunBurst
#		Bhvr	Behaviors/MachineGunBustOffset.bhvr
#		Part1	:MachineGunFlash.part
#		Part2	:MachineGunGlow.part
#	End
#
#	Event
#		EName 	Blast29a
#		Type	local
#		At	mystic
#		Geom	FX_MachineGunBurst
#		Bhvr	Behaviors/MachineGunBustOffsetL.bhvr
#		Part1	:MachineGunFlash.part
#		Part2	:MachineGunGlow.part
#	End
#
#End
#
#Condition
#	On 	Time
#	Time 	90
#
#	Event
#		EName 	Blast30
#		Type	local
#		At	mystic
#		Geom	FX_MachineGunBurst
#		Bhvr	Behaviors/MachineGunBustOffset.bhvr
#		Part1	:MachineGunFlash.part
#		Part2	:MachineGunGlow.part
#	End
#
#	Event
#		EName 	Blast30a
#		Type	local
#		At	mystic
#		Geom	FX_MachineGunBurst
#		Bhvr	Behaviors/MachineGunBustOffsetL.bhvr
#		Part1	:MachineGunFlash.part
#		Part2	:MachineGunGlow.part
#	End
#
#End
#
#Condition
#	On 	Time
#	Time 	93
#
#	Event
#		EName 	Blast31
#		Type	local
#		At	mystic
#		Geom	FX_MachineGunBurst
#		Bhvr	Behaviors/MachineGunBustOffset.bhvr
#		Part1	:MachineGunFlash.part
#		Part2	:MachineGunGlow.part
#	End
#
#	Event
#		EName 	Blast31a
#		Type	local
#		At	mystic
#		Geom	FX_MachineGunBurst
#		Bhvr	Behaviors/MachineGunBustOffsetL.bhvr
#		Part1	:MachineGunFlash.part
#		Part2	:MachineGunGlow.part
#	End
#
#End
#
#Condition
#	On 	Time
#	Time 	96
#
#	Event
#		EName 	Blast32
#		Type	local
#		At	mystic
#		Geom	FX_MachineGunBurst
#		Bhvr	Behaviors/MachineGunBustOffset.bhvr
#		Part1	:MachineGunFlash.part
#		Part2	:MachineGunGlow.part
#	End
#
#	Event
#		EName 	Blast32a
#		Type	local
#		At	mystic
#		Geom	FX_MachineGunBurst
#		Bhvr	Behaviors/MachineGunBustOffsetL.bhvr
#		Part1	:MachineGunFlash.part
#		Part2	:MachineGunGlow.part
#	End
#
#End
#
#Condition
#	On 	Time
#	Time 	99
#
#	Event
#		EName 	Blast33
#		Type	local
#		At	mystic
#		Geom	FX_MachineGunBurst
#		Bhvr	Behaviors/MachineGunBustOffset.bhvr
#		Part1	:MachineGunFlash.part
#		Part2	:MachineGunGlow.part
#	End
#
#	Event
#		EName 	Blast33a
#		Type	local
#		At	mystic
#		Geom	FX_MachineGunBurst
#		Bhvr	Behaviors/MachineGunBustOffsetL.bhvr
#		Part1	:MachineGunFlash.part
#		Part2	:MachineGunGlow.part
#	End
#
#
#End
#
#Condition
#	On 	Time
#	Time 	102
#
#	Event
#		EName 	Blast34
#		Type	local
#		At	mystic
#		Geom	FX_MachineGunBurst
#		Bhvr	Behaviors/MachineGunBustOffset.bhvr
#		Part1	:MachineGunFlash.part
#		Part2	:MachineGunGlow.part
#	End
#
#	Event
#		EName 	Blast34a
#		Type	local
#		At	mystic
#		Geom	FX_MachineGunBurst
#		Bhvr	Behaviors/MachineGunBustOffsetL.bhvr
#		Part1	:MachineGunFlash.part
#		Part2	:MachineGunGlow.part
#	End
#
#End
#
#Condition
#	On 	Time
#	Time 	105
#
#	Event
#		EName 	Blast35
#		Type	local
#		At	mystic
#		Geom	FX_MachineGunBurst
#		Bhvr	Behaviors/MachineGunBustOffset.bhvr
#		Part1	:MachineGunFlash.part
#		Part2	:MachineGunGlow.part
#	End
#
#	Event
#		EName 	Blast35a
#		Type	local
#		At	mystic
#		Geom	FX_MachineGunBurst
#		Bhvr	Behaviors/MachineGunBustOffsetL.bhvr
#		Part1	:MachineGunFlash.part
#		Part2	:MachineGunGlow.part
#	End
#
#End
#
#Condition
#	On 	Time
#	Time 	108
#
#	Event
#		EName 	Blast36
#		Type	local
#		At	mystic
#		Geom	FX_MachineGunBurst
#		Bhvr	Behaviors/MachineGunBustOffset.bhvr
#		Part1	:MachineGunFlash.part
#		Part2	:MachineGunGlow.part
#	End
#
#	Event
#		EName 	Blast36a
#		Type	local
#		At	mystic
#		Geom	FX_MachineGunBurst
#		Bhvr	Behaviors/MachineGunBustOffsetL.bhvr
#		Part1	:MachineGunFlash.part
#		Part2	:MachineGunGlow.part
#	End
#
#End
#
#Condition
#	On 	Time
#	Time 	111
#
#	Event
#		EName 	Blast37
#		Type	local
#		At	mystic
#		Geom	FX_MachineGunBurst
#		Bhvr	Behaviors/MachineGunBustOffset.bhvr
#		Part1	:MachineGunFlash.part
#		Part2	:MachineGunGlow.part
#	End
#
#	Event
#		EName 	Blast37a
#		Type	local
#		At	mystic
#		Geom	FX_MachineGunBurst
#		Bhvr	Behaviors/MachineGunBustOffsetL.bhvr
#		Part1	:MachineGunFlash.part
#		Part2	:MachineGunGlow.part
#	End
#
#End
#
#Condition
#	On 	Time
#	Time 	114
#
#	Event
#		EName 	Blast38
#		Type	local
#		At	mystic
#		Geom	FX_MachineGunBurst
#		Bhvr	Behaviors/MachineGunBustOffset.bhvr
#		Part1	:MachineGunFlash.part
#		Part2	:MachineGunGlow.part
#	End
#
#
#	Event
#		EName 	Blast38a
#		Type	local
#		At	mystic
#		Geom	FX_MachineGunBurst
#		Bhvr	Behaviors/MachineGunBustOffsetL.bhvr
#		Part1	:MachineGunFlash.part
#		Part2	:MachineGunGlow.part
#	End
#End
#
#Condition
#	On 	Time
#	Time 	117
#
#	Event
#		EName 	Blast39
#		Type	local
#		At	mystic
#		Geom	FX_MachineGunBurst
#		Bhvr	Behaviors/MachineGunBustOffset.bhvr
#		Part1	:MachineGunFlash.part
#		Part2	:MachineGunGlow.part
#	End
#
#	Event
#		EName 	Blast39a
#		Type	local
#		At	mystic
#		Geom	FX_MachineGunBurst
#		Bhvr	Behaviors/MachineGunBustOffsetL.bhvr
#		Part1	:MachineGunFlash.part
#		Part2	:MachineGunGlow.part
#	End
#
#End
#
#Condition
#	On 	Time
#	Time 	120
#
#	Event
#		EName 	Blast40
#		Type	local
#		At	mystic
#		Geom	FX_MachineGunBurst
#		Bhvr	Behaviors/MachineGunBustOffset.bhvr
#		Part1	:MachineGunFlash.part
#		Part2	:MachineGunGlow.part
#	End
#
#	Event
#		EName 	Blast40a
#		Type	local
#		At	mystic
#		Geom	FX_MachineGunBurst
#		Bhvr	Behaviors/MachineGunBustOffsetL.bhvr
#		Part1	:MachineGunFlash.part
#		Part2	:MachineGunGlow.part
#	End
#
#End
#
#Condition
#	On 	Time
#	Time 	123
#
#	Event
#		EName 	Blast41
#		Type	local
#		At	mystic
#		Geom	FX_MachineGunBurst
#		Bhvr	Behaviors/MachineGunBustOffset.bhvr
#		Part1	:MachineGunFlash.part
#		Part2	:MachineGunGlow.part
#	End
#
#	Event
#		EName 	Blast41a
#		Type	local
#		At	mystic
#		Geom	FX_MachineGunBurst
#		Bhvr	Behaviors/MachineGunBustOffsetL.bhvr
#		Part1	:MachineGunFlash.part
#		Part2	:MachineGunGlow.part
#	End
#
#End
#
#Condition
#	On 	Time
#	Time 	126
#
#	Event
#		EName 	Blast42
#		Type	local
#		At	mystic
#		Bhvr	Behaviors/MachineGunBustOffset.bhvr
#		
#		Part1	:MachineGunFlash.part
#		Part2	:MachineGunGlow.part
#	End
#
#	Event
#		EName 	Blast42a
#		Type	local
#		At	mystic
#		Bhvr	Behaviors/MachineGunBustOffsetL.bhvr
#		
#		Part1	:MachineGunFlash.part
#		Part2	:MachineGunGlow.part
#	End
#
#End
#
#Condition
#	On 	Time
#	Time 	129
#
#	Event
#		EName 	Blast43
#		Type	local
#		At	mystic
#		Bhvr	Behaviors/MachineGunBustOffset.bhvr
#		
#		Part1	:MachineGunFlash.part
#		Part2	:MachineGunGlow.part
#	End
#
#	Event
#		EName 	Blast43a
#		Type	local
#		At	mystic
#		Bhvr	Behaviors/MachineGunBustOffsetL.bhvr
#		
#		Part1	:MachineGunFlash.part
#		Part2	:MachineGunGlow.part
#	End
#
#End
#
##################################################################

Condition
	On 	Time
	Time	3

	Event
		Ename 	Blast1
		Type	Destroy
	End

	Event
		Ename 	Blast1a
		Type	Destroy
	End
End


Condition
	On 	Time
	Time	6

	Event
		Ename 	Blast2
		Type	Destroy
	End

	Event
		Ename 	Blast2a
		Type	Destroy
	End
End

Condition
	On 	Time
	Time	9

	Event
		Ename 	Blast3
		Type	Destroy
	End

	Event
		Ename 	Blast3a
		Type	Destroy
	End
End

Condition
	On 	Time
	Time	12

	Event
		Ename 	Blast4
		Type	Destroy
	End

	Event
		Ename 	Blast4a
		Type	Destroy
	End
End


Condition
	On 	Time
	Time	15

	Event
		Ename 	Blast5
		Type	Destroy
	End

	Event
		Ename 	Blast5a
		Type	Destroy
	End
End


Condition
	On 	Time
	Time	18

	Event
		Ename 	Blast6
		Type	Destroy
	End

	Event
		Ename 	Blast6a
		Type	Destroy
	End
End



Condition
	On 	Time
	Time	21

	Event
		Ename 	Blast5
		Type	Destroy
	End

	Event
		Ename 	Blast5a
		Type	Destroy
	End
End


Condition
	On 	Time
	Time	24

	Event
		Ename 	Blast6
		Type	Destroy
	End

	Event
		Ename 	Blast6a
		Type	Destroy
	End
End



Condition
	On 	Time
	Time	27

	Event
		Ename 	Blast7
		Type	Destroy
	End

	Event
		Ename 	Blast7a
		Type	Destroy
	End
End



Condition
	On 	Time
	Time	30

	Event
		Ename 	Blast8
		Type	Destroy
	End

	Event
		Ename 	Blast8a
		Type	Destroy
	End
End



Condition
	On 	Time
	Time	33

	Event
		Ename 	Blast9
		Type	Destroy
	End

	Event
		Ename 	Blast9a
		Type	Destroy
	End
End


Condition
	On 	Time
	Time	36

	Event
		Ename 	Blast10
		Type	Destroy
	End

	Event
		Ename 	Blast10a
		Type	Destroy
	End
End



Condition
	On 	Time
	Time	39

	Event
		Ename 	Blast11
		Type	Destroy
	End

	Event
		Ename 	Blast11a
		Type	Destroy
	End
End


Condition
	On 	Time
	Time	42

	Event
		Ename 	Blast12
		Type	Destroy
	End

	Event
		Ename 	Blast12a
		Type	Destroy
	End
End


Condition
	On 	Time
	Time	45

	Event
		Ename 	Blast13
		Type	Destroy
	End

	Event
		Ename 	Blast13a
		Type	Destroy
	End
End


Condition
	On 	Time
	Time	48

	Event
		Ename 	Blast14
		Type	Destroy
	End

	Event
		Ename 	Blast14a
		Type	Destroy
	End
End



Condition
	On 	Time
	Time	51

	Event
		Ename 	Blast15
		Type	Destroy
	End

	Event
		Ename 	Blast15a
		Type	Destroy
	End
End



Condition
	On 	Time
	Time	54

	Event
		Ename 	Blast16
		Type	Destroy
	End

	Event
		Ename 	Blast16a
		Type	Destroy
	End
End


#Condition
#	On 	Time
#	Time	57
#
#	Event
#		Ename 	Blast17
#		Type	Destroy
#	End
#
#	Event
#		Ename 	Blast17a
#		Type	Destroy
#	End
#End
#
#
#Condition
#	On 	Time
#	Time	60
#
#	Event
#		Ename 	Blast18
#		Type	Destroy
#	End
#
#	Event
#		Ename 	Blast18a
#		Type	Destroy
#	End
#End
#
#
#Condition
#	On 	Time
#	Time	63
#
#	Event
#		Ename 	Blast19
#		Type	Destroy
#	End
#
#	Event
#		Ename 	Blast19a
#		Type	Destroy
#	End
#End
#
#
#
#Condition
#	On 	Time
#	Time	66
#
#	Event
#		Ename 	Blast20
#		Type	Destroy
#	End
#
#	Event
#		Ename 	Blast20a
#		Type	Destroy
#	End
#End
#
#
#Condition
#	On 	Time
#	Time	69
#
#	Event
#		Ename 	Blast21
#		Type	Destroy
#	End
#
#	Event
#		Ename 	Blast21a
#		Type	Destroy
#	End
#End
#
#
#Condition
#	On 	Time
#	Time	72
#
#	Event
#		Ename 	Blast22
#		Type	Destroy
#	End
#
#	Event
#		Ename 	Blast22a
#		Type	Destroy
#	End
#End
#
#
#Condition
#	On 	Time
#	Time	74
#
#	Event
#		Ename 	Blast23
#		Type	Destroy
#	End
#
#	Event
#		Ename 	Blast23a
#		Type	Destroy
#	End
#End
#
#
#Condition
#	On 	Time
#	Time	77
#
#	Event
#		Ename 	Blast24
#		Type	Destroy
#	End
#
#	Event
#		Ename 	Blast24a
#		Type	Destroy
#	End
#End
#
#
#
#Condition
#	On 	Time
#	Time	80
#
#	Event
#		Ename 	Blast25
#		Type	Destroy
#	End
#
#	Event
#		Ename 	Blast25a
#		Type	Destroy
#	End
#End
#
#
#Condition
#	On 	Time
#	Time	83
#
#	Event
#		Ename 	Blast26
#		Type	Destroy
#	End
#
#	Event
#		Ename 	Blast26a
#		Type	Destroy
#	End
#End
#
#
#Condition
#	On 	Time
#	Time	86
#
#	Event
#		Ename 	Blast27
#		Type	Destroy
#	End
#
#	Event
#		Ename 	Blast27a
#		Type	Destroy
#	End
#End
#
#
#Condition
#	On 	Time
#	Time	89
#
#	Event
#		Ename 	Blast28
#		Type	Destroy
#	End
#
#	Event
#		Ename 	Blast28a
#		Type	Destroy
#	End
#End
#
#
#
#Condition
#	On 	Time
#	Time	92
#
#	Event
#		Ename 	Blast29
#		Type	Destroy
#	End
#
#	Event
#		Ename 	Blast29a
#		Type	Destroy
#	End
#End
#
#
#Condition
#	On 	Time
#	Time	95
#
#	Event
#		Ename 	Blast30
#		Type	Destroy
#	End
#
#	Event
#		Ename 	Blast30a
#		Type	Destroy
#	End
#End
#
#
#Condition
#	On 	Time
#	Time	98
#
#	Event
#		Ename 	Blast31
#		Type	Destroy
#	End
#
#	Event
#		Ename 	Blast31a
#		Type	Destroy
#	End
#End
#
#
#Condition
#	On 	Time
#	Time	101
#
#	Event
#		Ename 	Blast32
#		Type	Destroy
#	End
#
#	Event
#		Ename 	Blast32a
#		Type	Destroy
#	End
#End
#
#
#Condition
#	On 	Time
#	Time	104
#
#	Event
#		Ename 	Blast33
#		Type	Destroy
#	End
#
#	Event
#		Ename 	Blast33a
#		Type	Destroy
#	End
#End
#
#
#
#Condition
#	On 	Time
#	Time	107
#
#	Event
#		Ename 	Blast34
#		Type	Destroy
#	End
#
#	Event
#		Ename 	Blast34a
#		Type	Destroy
#	End
#End
#
#
#
#Condition
#	On 	Time
#	Time	110
#
#	Event
#		Ename 	Blast35
#		Type	Destroy
#	End
#
#	Event
#		Ename 	Blast35a
#		Type	Destroy
#	End
#End
#
#
#Condition
#	On 	Time
#	Time	113
#
#	Event
#		Ename 	Blast36
#		Type	Destroy
#	End
#
#	Event
#		Ename 	Blast36a
#		Type	Destroy
#	End
#End
#
#
#Condition
#	On 	Time
#	Time	116
#
#	Event
#		Ename 	Blast37
#		Type	Destroy
#	End
#
#	Event
#		Ename 	Blast37a
#		Type	Destroy
#	End
#End
#
#
#Condition
#	On 	Time
#	Time	119
#
#	Event
#		Ename 	Blast38
#		Type	Destroy
#	End
#
#	Event
#		Ename 	Blast38a
#		Type	Destroy
#	End
#End
#
#
#Condition
#	On 	Time
#	Time	122
#
#	Event
#		Ename 	Blast39
#		Type	Destroy
#	End
#
#	Event
#		Ename 	Blast39a
#		Type	Destroy
#	End
#End
#
#
#Condition
#	On 	Time
#	Time	125
#
#	Event
#		Ename 	Blast40
#		Type	Destroy
#	End
#
#	Event
#		Ename 	Blast40a
#		Type	Destroy
#	End
#End
#
#
#Condition
#	On 	Time
#	Time	128
#
#	Event
#		Ename 	Blast41
#		Type	Destroy
#	End
#
#	Event
#		Ename 	Blast41a
#		Type	Destroy
#	End
#End
#
#
#Condition
#	On 	Time
#	Time	131
#
#	Event
#		Ename 	Blast42
#		Type	Destroy
#	End
#
#	Event
#		Ename 	Blast42a
#		Type	Destroy
#	End
#End
#
#Condition
#	On 	Time
#	Time	134
#
#	Event
#		Ename 	Blast43
#		Type	Destroy
#	End
#
#	Event
#		Ename 	Blast43a
#		Type	Destroy
#	End
#End
#
Condition
	On 	Time
	Time	57

	Event
		Ename 	All
		Type	Destroy
	End
End


End				