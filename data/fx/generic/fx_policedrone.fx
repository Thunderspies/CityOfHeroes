#########################################################
##	template

FxInfo

Input  
	Inpname	head
End

Input  
	Inpname	HandL
End

Input  
	Inpname	HandR
End

Input  
	Inpname	FootL
End

Input  
	Inpname	FootR
End

Input  
	Inpname	Hair
End

Input  
	Inpname	chest
End


Input  
	Inpname	origin
End

Flags	InheritAlpha
Condition

	On 	Time
	Time 	0

	Event
		Type	Local
		At	origin
		Sound policedrone2_loop 100 100 .18
	End
End

Condition
	On 	Time
	Time 	0

	Event
		EName	Glass
		Type	Local
		At	chest
		Geom	PoliceGlass
					
	End

	Event
		EName	Spotlight01
		Type	Local
		At	Glass
		altpiv	1
		#Part1	:SpotLightRays01.part
					
	End

	Event
		EName	Spotlight01
		Type	Local
		At	Glass
		altpiv	2
		#Part1	:SpotLightRays02.part
					
	End

	Event
		Ename	Hookup
		Type	Local
		At	HandL
		
		Part1	:RedLightCenter.part
		Part2	:RedLightGlow.part

	End

	Event
		Ename	LowerRightLight
		Type	Local
		At	HandR
		
		Part1	:RedLightCenter.part
		Part2	:RedLightGlow.part

	End

End

Condition
	On	time
	Time 	0


	Event
		Ename	leftLampBlue
		Type	Local
		At	FootR
		
		Part1	:RedLightCenter2.part
		Part2	:RedLightGlow1.part
		Part3	:RedLightGlow2.part
		Part4	:RedLightRing.part
		

	End

End

Condition
	On	time
	Time 	15

	Event
		Ename	leftLampBlue
		Type	Local
		At	FootL
		
		Part1	:BlueLightCenter2.part
		Part2	:BlueLightGlow1.part
		Part3	:BlueLightGlow2.part
		Part4	:BlueLightRing.part

	End

	
End

Condition
	On 	Time
	Time 	15

	Event
		Ename	TopleftLamp
		Type	Local
		At	hair
		
		Part1	:LightCenter.part
		Part2	:LightGlow.part

	End

	Event
		Ename	TopRightLamp
		Type	Local
		At	head
		
		Part1	:AntennaSpark.part
		#Part2	:RedLightGlow.part

	End

End

End		