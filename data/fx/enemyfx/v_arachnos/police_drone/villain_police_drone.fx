#########################################################
##	template

FxInfo

Flags    InheritAlpha

#########################################################
####################### AUDIO ###########################
#########################################################

Condition
	On 	Time
	Time 	10

	Event
		Type	Local
		At	Hips	
		Sound PoliceBot1_loop 90 90 .32
		
	End
End

Condition
	On 	Time
	Time 	10

	Event
		Type	Local
		At	Hips	
		#Sound	PoliceBot2_loop 90 80 .1
		
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
		part2	:BodyAura01.part
		bhvr	:eyes_right_01.bhvr
		
	End

	Event
		EName	Aura
		Type	Local
		At	Hips	
		part2	:BodyAura01.part
		bhvr	:eyes_right_02.bhvr
		
	End

	Event
		EName	Aura
		Type	Local
		At	Hips	
		part2	:BodyAura01.part
		bhvr	:eyes_right_03.bhvr
		
	End

	Event
		EName	Aura
		Type	Local
		At	Hips	
		part2	:BodyAura03.part
		bhvr	:eyes_right_04.bhvr
		
	End

	Event
		EName	Aura
		Type	Local
		At	Hips	
		part2	:BodyAura01.part
		bhvr	:eyes_right_05.bhvr
		
	End

	Event
		EName	Aura
		Type	Local
		At	Hips	
		part2	:BodyAura01.part
		bhvr	:eyes_right_06.bhvr
		
	End

	Event
		EName	Aura
		Type	Local
		At	Hips	
		part2	:BodyAura01.part
		bhvr	:eyes_right_07.bhvr
		
	End

	Event
		EName	Aura
		Type	Local
		At	Hips	
		part2	:BodyAura03.part
		bhvr	:eyes_right_08.bhvr
		
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
		Anim	V_FX_Arachno_Police_legs
		
	End


End


End

