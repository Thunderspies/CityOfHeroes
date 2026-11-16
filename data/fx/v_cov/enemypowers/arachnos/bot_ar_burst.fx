#########################################################
##	
##

FxInfo
LifeSpan  150

Input  
	InpName	Target
End

Input  
	InpName	Mystic
End

Input  
	InpName	WepR
End

##################################
Condition
	On 	Time
	Time 	42

	Event
		Type	Local 
		At	WepR
		Sound Assault3 120.0 120.0 .7
	End

end



Condition
	On 	Time
	Time 	38

	Event
		ENAME   p2
		Type	Local 
		At	c_wepL
		#bhvr	:Bot_Rot90.bhvr
		#Part1	:Bot_ARShell.Part
		ChildFX	V_COV\PhysicsEnabled\BotGunShellEject.fx
		BhvrOverride
			PyrRotate		0 180 0
		End
		lifespan 120 #3
	End

	Event
		EName 	Blast1
		Type	local
		At	c_wepL
		Geom	FX_MachineGunBurst
		bhvr	:Bot_AR_Rotforward.bhvr
		Part1	weaponfx\MachineGunFlash.part
		Part2	weaponfx\MachineGunGlow.part
		lifespan 3
	End

	Event
		EName 	tracer
		Type	local
		At	c_wepL
		bhvr	:Bot_AR_Rotforward.bhvr
		Part1	WEAPONFX/TorchflameA.part
		Part2	WEAPONFX/TorchflameB.part
		lifespan 3
	End


End

Condition
	On 	Time
	Time 	42

	Event
		EName 	Blast2
		Type	local
		At	c_wepR
		Geom	FX_MachineGunBurst
		bhvr	:Bot_AR_Rotforward.bhvr
		Part1	weaponfx\MachineGunFlash.part
		Part2	weaponfx\MachineGunGlow.part
		lifespan 3
	End


	Event
		EName 	tracer
		Type	local
		At	c_wepR
		bhvr	:Bot_AR_Rotforward.bhvr
		Part1	WEAPONFX/TorchflameA.part
		Part2	WEAPONFX/TorchflameB.part
		lifespan 3
	End

	Event
		Type	Local 
		At	c_wepR
		#bhvr	:Bot_Rot-90.bhvr
		#Part1	:Bot_ARShell.Part
		ChildFX	V_COV\PhysicsEnabled\BotGunShellEject.fx
		BhvrOverride
			#PyrRotate		0 90 0
		End
		lifespan 120 #3
	End


End


Condition
	On 	Time
	Time 	46

	Event
		EName 	Blast3
		Type	local
		At	c_wepL
		Geom	FX_MachineGunBurst
		bhvr	:Bot_AR_Rotforward.bhvr
		Part1	weaponfx\MachineGunFlash.part
		Part2	weaponfx\MachineGunGlow.part
		lifespan 3
	End

	Event
		EName 	tracer
		Type	local
		At	c_wepL
		bhvr	:Bot_AR_Rotforward.bhvr
		Part1	WEAPONFX/TorchflameA.part
		Part2	WEAPONFX/TorchflameB.part
		lifespan 3
	End

	

	Event
		Type	Local 
		At	c_wepL
		#bhvr	:Bot_Rot90.bhvr
		#Part1	:Bot_ARShell.Part
		ChildFX	V_COV\PhysicsEnabled\BotGunShellEject.fx
		BhvrOverride
			PyrRotate		0 180 0
		End
		lifespan 120 #3
	End
End

#Condition
#	On 	Time
#	Time 	38
#
#	Event
#		EName 	Blast4
#		Type	local
#		At	p2
#		Altpiv 1
#		Geom	FX_MachineGunBurst
#		Part1	weaponfx\MachineGunFlash.part
#		Part2	weaponfx\MachineGunGlow.part
#	End
#
#End
#
#Condition
#	On 	Time
#	Time 	41
#
#	Event
#		EName 	Blast5
#		Type	local
#		At	p2
#		Altpiv 1
#		Geom	FX_MachineGunBurst
#		Part1	weaponfx\MachineGunFlash.part
#		Part2	weaponfx\MachineGunGlow.part
#	End
#
#End




##################################

#Condition
#	On 	Time
#	Time	34
#
#	Event
#		Ename 	Blast1
#		Type	Destroy
#	End
#End
#
#
#Condition
#	On 	Time
#	Time	37
#
#	Event
#		Ename 	Blast2
#		Type	Destroy
#	End
#End
#
#Condition
#	On 	Time
#	Time	40
#
#	Event
#		Ename 	Blast3
#		Type	Destroy
#	End
#End
#
#Condition
#	On 	Time
#	Time	43
#
#	Event
#		Ename 	Blast4
#		Type	Destroy
#	End
#End
#
#Condition
#	On 	Time
#	Time	45
#
#	Event
#		Ename 	All
#		Type	Destroy
#	End
#End

End				