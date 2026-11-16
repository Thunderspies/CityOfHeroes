#########################################################
##	
##

FxInfo
LifeSpan  250

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
	Time 	38

	Event
		ename	ShellsL
		Type	Local 
		At	c_wepL
		bhvr	:Bot_Rot90.bhvr
#		Geom	FlameThrower
#		Part1	:Bot_ARShell.Part
		Sound machgun3 120.0 120.0 .9
#		lifespan 3
	End

	Event
		EName 	tracerL
		Type	local
		At	c_wepL
		bhvr	:Bot_AR_Rotforward.bhvr
		Part1	WEAPONFX/TorchflameA.part
		Part2	WEAPONFX/TorchflameB.part
#		lifespan 3
		
	End


	Event
		EName 	HeatL
		Type	local
		At	c_wepL
#		Geom	FX_MachineGunBurst
#		bhvr	:Bot_AR_Rotforward.bhvr
		Part1	:Bot_Gunheat.Part
#		Part1	weaponfx\MachineGunFlash.part
#		Part2	weaponfx\MachineGunGlow.part
#		lifespan 3
		pother tracerL
	End

End

Condition
	On 	Time
	Time 	42




	Event
		EName 	tracerR
		Type	local
		At	c_wepR
		bhvr	:Bot_AR_Rotforward.bhvr
		Part1	WEAPONFX/TorchflameA.part
		Part2	WEAPONFX/TorchflameB.part
#		lifespan 3
	End

	Event
		EName 	HeatR
		Type	local
		At	c_wepR
		
		Part1	:Bot_Gunheat.Part
#		Geom	FX_MachineGunBurst
#		bhvr	:Bot_AR_Rotforward.bhvr
#		Part1	weaponfx\MachineGunFlash.part
#		Part2	weaponfx\MachineGunGlow.part
#		lifespan 3
		pother tracerR
	End

	Event
		ename	ShellsR
		Type	Local 
		At	c_wepR
		bhvr	:Bot_Rot-90.bhvr
#		Part1	:Bot_ARShell.Part
#		lifespan 3
	End


End


#Condition
#	On 	Time
#	Time 	46
#
#	Event
#		EName 	Blast3
#		Type	local
#		At	c_wepL
#		Geom	FX_MachineGunBurst
#		bhvr	:Bot_AR_Rotforward.bhvr
#		Part1	weaponfx\MachineGunFlash.part
#		Part2	weaponfx\MachineGunGlow.part
##		lifespan 3
#	End
#
#	Event
#		EName 	tracer
#		Type	local
#		At	c_wepL
#		bhvr	:Bot_AR_Rotforward.bhvr
#		Part1	WEAPONFX/TorchflameA.part
#		Part2	WEAPONFX/TorchflameB.part
##		lifespan 3
#	End
#
#	
#
#	Event
#		Type	Local 
#		At	c_wepL
#		bhvr	:Bot_Rot90.bhvr
#		Part1	:Bot_ARShell.Part
##		lifespan 3
#	End
#End
#
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



Condition
	On 	Time
	Time 	38

	Event
		ename	ShellL
		Type	Start 
		At	c_wepL
		ChildFX	V_COV\PhysicsEnabled\BotGunShellEjectL.fx
		lifespan 60
	End
End
Condition
	On 	Time
	Time 	41

	Event
		ename	ShellL
		Type	Start 
		At	c_wepL
		ChildFX	V_COV\PhysicsEnabled\BotGunShellEjectL.fx
		lifespan 60
	End
End
Condition
	On 	Time
	Time 	44

	Event
		ename	ShellL
		Type	Start 
		At	c_wepL
		ChildFX	V_COV\PhysicsEnabled\BotGunShellEjectL.fx
		lifespan 60
	End
End
Condition
	On 	Time
	Time 	47

	Event
		ename	ShellL
		Type	Start 
		At	c_wepL
		ChildFX	V_COV\PhysicsEnabled\BotGunShellEjectL.fx
		lifespan 60
	End
End
Condition
	On 	Time
	Time 	50

	Event
		ename	ShellL
		Type	Start 
		At	c_wepL
		ChildFX	V_COV\PhysicsEnabled\BotGunShellEjectL.fx
		lifespan 60
	End
End
Condition
	On 	Time
	Time 	53

	Event
		ename	ShellL
		Type	Start 
		At	c_wepL
		ChildFX	V_COV\PhysicsEnabled\BotGunShellEjectL.fx
		lifespan 60
	End
End
Condition
	On 	Time
	Time 	56

	Event
		ename	ShellL
		Type	Start 
		At	c_wepL
		ChildFX	V_COV\PhysicsEnabled\BotGunShellEjectL.fx
		lifespan 60
	End
End
Condition
	On 	Time
	Time 	59

	Event
		ename	ShellL
		Type	Start 
		At	c_wepL
		ChildFX	V_COV\PhysicsEnabled\BotGunShellEjectL.fx
		lifespan 60
	End
End
Condition
	On 	Time
	Time 	62

	Event
		ename	ShellL
		Type	Start 
		At	c_wepL
		ChildFX	V_COV\PhysicsEnabled\BotGunShellEjectL.fx
		lifespan 60
	End
End
Condition
	On 	Time
	Time 	65

	Event
		ename	ShellL
		Type	Start 
		At	c_wepL
		ChildFX	V_COV\PhysicsEnabled\BotGunShellEjectL.fx
		lifespan 60
	End
End
Condition
	On 	Time
	Time 	68

	Event
		ename	ShellL
		Type	Start 
		At	c_wepL
		ChildFX	V_COV\PhysicsEnabled\BotGunShellEjectL.fx
		lifespan 60
	End
End
Condition
	On 	Time
	Time 	71

	Event
		ename	ShellL
		Type	Start 
		At	c_wepL
		ChildFX	V_COV\PhysicsEnabled\BotGunShellEjectL.fx
		lifespan 60
	End
End
Condition
	On 	Time
	Time 	74

	Event
		ename	ShellL
		Type	Start 
		At	c_wepL
		ChildFX	V_COV\PhysicsEnabled\BotGunShellEjectL.fx
		lifespan 60
	End
End
Condition
	On 	Time
	Time 	77

	Event
		ename	ShellL
		Type	Start 
		At	c_wepL
		ChildFX	V_COV\PhysicsEnabled\BotGunShellEjectL.fx
		lifespan 60
	End
End
Condition
	On 	Time
	Time 	80

	Event
		ename	ShellL
		Type	Start 
		At	c_wepL
		ChildFX	V_COV\PhysicsEnabled\BotGunShellEjectL.fx
		lifespan 60
	End
End
Condition
	On 	Time
	Time 	83

	Event
		ename	ShellL
		Type	Start 
		At	c_wepL
		ChildFX	V_COV\PhysicsEnabled\BotGunShellEjectL.fx
		lifespan 60
	End
End
Condition
	On 	Time
	Time 	86

	Event
		ename	ShellL
		Type	Start 
		At	c_wepL
		ChildFX	V_COV\PhysicsEnabled\BotGunShellEjectL.fx
		lifespan 60
	End
End
Condition
	On 	Time
	Time 	89

	Event
		ename	ShellL
		Type	Start 
		At	c_wepL
		ChildFX	V_COV\PhysicsEnabled\BotGunShellEjectL.fx
		lifespan 60
	End
End
Condition
	On 	Time
	Time 	92

	Event
		ename	ShellL
		Type	Start 
		At	c_wepL
		ChildFX	V_COV\PhysicsEnabled\BotGunShellEjectL.fx
		lifespan 60
	End
End
Condition
	On 	Time
	Time 	95

	Event
		ename	ShellL
		Type	Start 
		At	c_wepL
		ChildFX	V_COV\PhysicsEnabled\BotGunShellEjectL.fx
		lifespan 60
	End
End
Condition
	On 	Time
	Time 	98

	Event
		ename	ShellL
		Type	Start 
		At	c_wepL
		ChildFX	V_COV\PhysicsEnabled\BotGunShellEjectL.fx
		lifespan 60
	End
End
Condition
	On 	Time
	Time 	101

	Event
		ename	ShellL
		Type	Start 
		At	c_wepL
		ChildFX	V_COV\PhysicsEnabled\BotGunShellEjectL.fx
		lifespan 60
	End
End
Condition
	On 	Time
	Time 	104

	Event
		ename	ShellL
		Type	Start 
		At	c_wepL
		ChildFX	V_COV\PhysicsEnabled\BotGunShellEjectL.fx
		lifespan 60
	End
End
Condition
	On 	Time
	Time 	107

	Event
		ename	ShellL
		Type	Start 
		At	c_wepL
		ChildFX	V_COV\PhysicsEnabled\BotGunShellEjectL.fx
		lifespan 60
	End
End
Condition
	On 	Time
	Time 	110

	Event
		ename	ShellL
		Type	Start 
		At	c_wepL
		ChildFX	V_COV\PhysicsEnabled\BotGunShellEjectL.fx
		lifespan 60
	End
End
Condition
	On 	Time
	Time 	113

	Event
		ename	ShellL
		Type	Start 
		At	c_wepL
		ChildFX	V_COV\PhysicsEnabled\BotGunShellEjectL.fx
		lifespan 60
	End
End
Condition
	On 	Time
	Time 	116

	Event
		ename	ShellL
		Type	Start 
		At	c_wepL
		ChildFX	V_COV\PhysicsEnabled\BotGunShellEjectL.fx
		lifespan 60
	End
End
Condition
	On 	Time
	Time 	119

	Event
		ename	ShellL
		Type	Start 
		At	c_wepL
		ChildFX	V_COV\PhysicsEnabled\BotGunShellEjectL.fx
		lifespan 60
	End
End
Condition
	On 	Time
	Time 	122

	Event
		ename	ShellL
		Type	Start 
		At	c_wepL
		ChildFX	V_COV\PhysicsEnabled\BotGunShellEjectL.fx
		lifespan 60
	End
End
Condition
	On 	Time
	Time 	125

	Event
		ename	ShellL
		Type	Start 
		At	c_wepL
		ChildFX	V_COV\PhysicsEnabled\BotGunShellEjectL.fx
		lifespan 60
	End
End
Condition
	On 	Time
	Time 	128

	Event
		ename	ShellL
		Type	Start 
		At	c_wepL
		ChildFX	V_COV\PhysicsEnabled\BotGunShellEjectL.fx
		lifespan 60
	End
End
Condition
	On 	Time
	Time 	131

	Event
		ename	ShellL
		Type	Start 
		At	c_wepL
		ChildFX	V_COV\PhysicsEnabled\BotGunShellEjectL.fx
		lifespan 60
	End
End
Condition
	On 	Time
	Time 	134

	Event
		ename	ShellL
		Type	Start 
		At	c_wepL
		ChildFX	V_COV\PhysicsEnabled\BotGunShellEjectL.fx
		lifespan 60
	End
End
Condition
	On 	Time
	Time 	137

	Event
		ename	ShellL
		Type	Start 
		At	c_wepL
		ChildFX	V_COV\PhysicsEnabled\BotGunShellEjectL.fx
		lifespan 60
	End
End
Condition
	On 	Time
	Time 	140

	Event
		ename	ShellL
		Type	Start 
		At	c_wepL
		ChildFX	V_COV\PhysicsEnabled\BotGunShellEjectL.fx
		lifespan 60
	End
End
Condition
	On 	Time
	Time 	143

	Event
		ename	ShellL
		Type	Start 
		At	c_wepL
		ChildFX	V_COV\PhysicsEnabled\BotGunShellEjectL.fx
		lifespan 60
	End
End
Condition
	On 	Time
	Time 	146

	Event
		ename	ShellL
		Type	Start 
		At	c_wepL
		ChildFX	V_COV\PhysicsEnabled\BotGunShellEjectL.fx
		lifespan 60
	End
End
Condition
	On 	Time
	Time 	149

	Event
		ename	ShellL
		Type	Start 
		At	c_wepL
		ChildFX	V_COV\PhysicsEnabled\BotGunShellEjectL.fx
		lifespan 60
	End
End
Condition
	On 	Time
	Time 	152

	Event
		ename	ShellL
		Type	Start 
		At	c_wepL
		ChildFX	V_COV\PhysicsEnabled\BotGunShellEjectL.fx
		lifespan 60
	End
End
Condition
	On 	Time
	Time 	155

	Event
		ename	ShellL
		Type	Start 
		At	c_wepL
		ChildFX	V_COV\PhysicsEnabled\BotGunShellEjectL.fx
		lifespan 60
	End
End
Condition
	On 	Time
	Time 	158

	Event
		ename	ShellL
		Type	Start 
		At	c_wepL
		ChildFX	V_COV\PhysicsEnabled\BotGunShellEjectL.fx
		lifespan 60
	End
End

Condition
	On 	Time
	Time 	42

	Event
		ename	ShellR
		Type	Local 
		At	c_wepR
		ChildFX	V_COV\PhysicsEnabled\BotGunShellEjectR.fx
		lifespan 60
	End
End
Condition
	On 	Time
	Time 	45

	Event
		ename	ShellR
		Type	Local 
		At	c_wepR
		ChildFX	V_COV\PhysicsEnabled\BotGunShellEjectR.fx
		lifespan 60
	End
End
Condition
	On 	Time
	Time 	48

	Event
		ename	ShellR
		Type	Local 
		At	c_wepR
		ChildFX	V_COV\PhysicsEnabled\BotGunShellEjectR.fx
		lifespan 60
	End
End
Condition
	On 	Time
	Time 	51

	Event
		ename	ShellR
		Type	Local 
		At	c_wepR
		ChildFX	V_COV\PhysicsEnabled\BotGunShellEjectR.fx
		lifespan 60
	End
End
Condition
	On 	Time
	Time 	54

	Event
		ename	ShellR
		Type	Local 
		At	c_wepR
		ChildFX	V_COV\PhysicsEnabled\BotGunShellEjectR.fx
		lifespan 60
	End
End
Condition
	On 	Time
	Time 	57

	Event
		ename	ShellR
		Type	Local 
		At	c_wepR
		ChildFX	V_COV\PhysicsEnabled\BotGunShellEjectR.fx
		lifespan 60
	End
End
Condition
	On 	Time
	Time 	60

	Event
		ename	ShellR
		Type	Local 
		At	c_wepR
		ChildFX	V_COV\PhysicsEnabled\BotGunShellEjectR.fx
		lifespan 60
	End
End
Condition
	On 	Time
	Time 	63

	Event
		ename	ShellR
		Type	Local 
		At	c_wepR
		ChildFX	V_COV\PhysicsEnabled\BotGunShellEjectR.fx
		lifespan 60
	End
End
Condition
	On 	Time
	Time 	66

	Event
		ename	ShellR
		Type	Local 
		At	c_wepR
		ChildFX	V_COV\PhysicsEnabled\BotGunShellEjectR.fx
		lifespan 60
	End
End
Condition
	On 	Time
	Time 	69

	Event
		ename	ShellR
		Type	Local 
		At	c_wepR
		ChildFX	V_COV\PhysicsEnabled\BotGunShellEjectR.fx
		lifespan 60
	End
End
Condition
	On 	Time
	Time 	72

	Event
		ename	ShellR
		Type	Local 
		At	c_wepR
		ChildFX	V_COV\PhysicsEnabled\BotGunShellEjectR.fx
		lifespan 60
	End
End
Condition
	On 	Time
	Time 	75

	Event
		ename	ShellR
		Type	Local 
		At	c_wepR
		ChildFX	V_COV\PhysicsEnabled\BotGunShellEjectR.fx
		lifespan 60
	End
End
Condition
	On 	Time
	Time 	78

	Event
		ename	ShellR
		Type	Local 
		At	c_wepR
		ChildFX	V_COV\PhysicsEnabled\BotGunShellEjectR.fx
		lifespan 60
	End
End
Condition
	On 	Time
	Time 	81

	Event
		ename	ShellR
		Type	Local 
		At	c_wepR
		ChildFX	V_COV\PhysicsEnabled\BotGunShellEjectR.fx
		lifespan 60
	End
End
Condition
	On 	Time
	Time 	84

	Event
		ename	ShellR
		Type	Local 
		At	c_wepR
		ChildFX	V_COV\PhysicsEnabled\BotGunShellEjectR.fx
		lifespan 60
	End
End
Condition
	On 	Time
	Time 	87

	Event
		ename	ShellR
		Type	Local 
		At	c_wepR
		ChildFX	V_COV\PhysicsEnabled\BotGunShellEjectR.fx
		lifespan 60
	End
End
Condition
	On 	Time
	Time 	90

	Event
		ename	ShellR
		Type	Local 
		At	c_wepR
		ChildFX	V_COV\PhysicsEnabled\BotGunShellEjectR.fx
		lifespan 60
	End
End
Condition
	On 	Time
	Time 	93

	Event
		ename	ShellR
		Type	Local 
		At	c_wepR
		ChildFX	V_COV\PhysicsEnabled\BotGunShellEjectR.fx
		lifespan 60
	End
End
Condition
	On 	Time
	Time 	96

	Event
		ename	ShellR
		Type	Local 
		At	c_wepR
		ChildFX	V_COV\PhysicsEnabled\BotGunShellEjectR.fx
		lifespan 60
	End
End
Condition
	On 	Time
	Time 	99

	Event
		ename	ShellR
		Type	Local 
		At	c_wepR
		ChildFX	V_COV\PhysicsEnabled\BotGunShellEjectR.fx
		lifespan 60
	End
End
Condition
	On 	Time
	Time 	102

	Event
		ename	ShellR
		Type	Local 
		At	c_wepR
		ChildFX	V_COV\PhysicsEnabled\BotGunShellEjectR.fx
		lifespan 60
	End
End
Condition
	On 	Time
	Time 	105

	Event
		ename	ShellR
		Type	Local 
		At	c_wepR
		ChildFX	V_COV\PhysicsEnabled\BotGunShellEjectR.fx
		lifespan 60
	End
End
Condition
	On 	Time
	Time 	108

	Event
		ename	ShellR
		Type	Local 
		At	c_wepR
		ChildFX	V_COV\PhysicsEnabled\BotGunShellEjectR.fx
		lifespan 60
	End
End
Condition
	On 	Time
	Time 	111

	Event
		ename	ShellR
		Type	Local 
		At	c_wepR
		ChildFX	V_COV\PhysicsEnabled\BotGunShellEjectR.fx
		lifespan 60
	End
End
Condition
	On 	Time
	Time 	114

	Event
		ename	ShellR
		Type	Local 
		At	c_wepR
		ChildFX	V_COV\PhysicsEnabled\BotGunShellEjectR.fx
		lifespan 60
	End
End
Condition
	On 	Time
	Time 	117

	Event
		ename	ShellR
		Type	Local 
		At	c_wepR
		ChildFX	V_COV\PhysicsEnabled\BotGunShellEjectR.fx
		lifespan 60
	End
End
Condition
	On 	Time
	Time 	120

	Event
		ename	ShellR
		Type	Local 
		At	c_wepR
		ChildFX	V_COV\PhysicsEnabled\BotGunShellEjectR.fx
		lifespan 60
	End
End
Condition
	On 	Time
	Time 	123

	Event
		ename	ShellR
		Type	Local 
		At	c_wepR
		ChildFX	V_COV\PhysicsEnabled\BotGunShellEjectR.fx
		lifespan 60
	End
End
Condition
	On 	Time
	Time 	126

	Event
		ename	ShellR
		Type	Local 
		At	c_wepR
		ChildFX	V_COV\PhysicsEnabled\BotGunShellEjectR.fx
		lifespan 60
	End
End
Condition
	On 	Time
	Time 	129

	Event
		ename	ShellR
		Type	Local 
		At	c_wepR
		ChildFX	V_COV\PhysicsEnabled\BotGunShellEjectR.fx
		lifespan 60
	End
End
Condition
	On 	Time
	Time 	132

	Event
		ename	ShellR
		Type	Local 
		At	c_wepR
		ChildFX	V_COV\PhysicsEnabled\BotGunShellEjectR.fx
		lifespan 60
	End
End
Condition
	On 	Time
	Time 	135

	Event
		ename	ShellR
		Type	Local 
		At	c_wepR
		ChildFX	V_COV\PhysicsEnabled\BotGunShellEjectR.fx
		lifespan 60
	End
End
Condition
	On 	Time
	Time 	138

	Event
		ename	ShellR
		Type	Local 
		At	c_wepR
		ChildFX	V_COV\PhysicsEnabled\BotGunShellEjectR.fx
		lifespan 60
	End
End
Condition
	On 	Time
	Time 	141

	Event
		ename	ShellR
		Type	Local 
		At	c_wepR
		ChildFX	V_COV\PhysicsEnabled\BotGunShellEjectR.fx
		lifespan 60
	End
End
Condition
	On 	Time
	Time 	144

	Event
		ename	ShellR
		Type	Local 
		At	c_wepR
		ChildFX	V_COV\PhysicsEnabled\BotGunShellEjectR.fx
		lifespan 60
	End
End
Condition
	On 	Time
	Time 	147

	Event
		ename	ShellR
		Type	Local 
		At	c_wepR
		ChildFX	V_COV\PhysicsEnabled\BotGunShellEjectR.fx
		lifespan 60
	End
End
Condition
	On 	Time
	Time 	150

	Event
		ename	ShellR
		Type	Local 
		At	c_wepR
		ChildFX	V_COV\PhysicsEnabled\BotGunShellEjectR.fx
		lifespan 60
	End
End
Condition
	On 	Time
	Time 	153

	Event
		ename	ShellR
		Type	Local 
		At	c_wepR
		ChildFX	V_COV\PhysicsEnabled\BotGunShellEjectR.fx
		lifespan 60
	End
End
Condition
	On 	Time
	Time 	156

	Event
		ename	ShellR
		Type	Local 
		At	c_wepR
		ChildFX	V_COV\PhysicsEnabled\BotGunShellEjectR.fx
		lifespan 60
	End
End
Condition
	On 	Time
	Time 	159

	Event
		ename	ShellR
		Type	Local 
		At	c_wepR
		ChildFX	V_COV\PhysicsEnabled\BotGunShellEjectR.fx
		lifespan 60
	End
End


##################################
Condition
	On 	Time
	Time	160

	Event
		Ename 	shellsR
		Type	Destroy
	End
End

Condition
	On 	Time
	Time	160

	Event
		Ename 	shellsL
		Type	Destroy
	End
End

Condition
	On 	Time
	Time	160

	Event
		Ename 	TracerR
		Type	Destroy
	End
End

Condition
	On 	Time
	Time	160

	Event
		Ename 	TracerL
		Type	Destroy
	End
End


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