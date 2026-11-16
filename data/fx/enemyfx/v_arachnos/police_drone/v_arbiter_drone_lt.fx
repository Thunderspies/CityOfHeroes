#########################################################
##	template

FxInfo

Flags    InheritAlpha

#########################################################
####################### AUDIO ###########################
#########################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	origin
		Sound ArbiterDrone_loop 60 60 .3
	End	

End

#########################################################
#################### BODY GLOW 1 ########################
#########################################################

Condition
	On 	Time
	Time 	10

	Event
		EName	Aura
		Type	Local
		At	Hips	
		part2	:Arbiter_Aura.part
		bhvr	:eyes_right_LT_01.bhvr
		
		
	End

	Event
		EName	Aura
		Type	Local
		At	Hips	
		part2	:Arbiter_Aura.part
		bhvr	:eyes_right_LT_02.bhvr
		
	End

	Event
		EName	Aura
		Type	Local
		At	Hips	
		part2	:Arbiter_Aura.part
		bhvr	:eyes_right_LT_03.bhvr
		
	End

	Event
		EName	Aura
		Type	Local
		At	Hips	
		part2	:Arbter_Aura_01.part
		bhvr	:eyes_right_LT_04.bhvr
		
	End

	Event
		EName	Aura
		Type	Local
		At	Hips	
		part2	:Arbiter_Aura.part
		bhvr	:eyes_right_LT_05.bhvr
		
	End

	Event
		EName	Aura
		Type	Local
		At	Hips	
		part2	:Arbiter_Aura.part
		bhvr	:eyes_right_LT_06.bhvr
		
	End

	Event
		EName	Aura
		Type	Local
		At	Hips	
		part2	:Arbiter_Aura.part
		bhvr	:eyes_right_LT_07.bhvr
		
	End

	Event
		EName	Aura
		Type	Local
		At	Hips	
		part2	:Arbter_Aura_01.part
		bhvr	:eyes_right_LT_08.bhvr
		
	End

End

#########################################################
#################### Animated Legs ########################
#########################################################

Condition
	On 	Time
	Time 	0

	Event
		EName	Legs
		Type	Local
		At	Waist
		Anim	V_FX_Arachno_Police_legs_Lt
		
	End


End

End