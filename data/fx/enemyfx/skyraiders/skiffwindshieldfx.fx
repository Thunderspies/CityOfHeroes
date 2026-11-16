#########################################################
##	Fireweapon
########################################################
FxInfo

Input  
	Inpname	HandL
End


Input  
	Inpname	FootL
End

Input  
	Inpname	FootR
End


Input  
	Inpname	T1_L
End

Input  
	Inpname	T1_R
End

Input  
	Inpname	T2_L
End

Input  
	Inpname	T2_R
End

Input  
	Inpname	T3_R
End

Input  
	Inpname	RingL
End

Input  
	Inpname	ToeL
End

Input  
	Inpname	F1_L
End

Input  
	Inpname	F2_L
End

Input  
	Inpname	RingR
End

Input  
	Inpname	ToeR
End

Input  
	Inpname	F1_R
End

Input  
	Inpname	F2_R
End
 

Flags    InheritAlpha


########################################################
## WindShield
########################################################

Condition
	On 	Time
	Time 	0

	Event
		EName 	Windshield
		Type	Local 
		At	HandL
		Geom	Fx_Face
	
	End

###########################################################
##Missle Effect
############################################################
	Event
		EName 	MissleSmoke
		Type	Local 
		At	Windshield
		  Altpiv  3
		#Part1	:MissleSmoke.part
		Sound Thruster_loop 150 150 .28
	End

	Event
		EName 	MissleSmoke
		Type	Local 
		At	Windshield
		  Altpiv  4
		#Part1	:MissleSmoke.part
		Sound forcefield5_loop 110 110 .75
	End

	Event
		EName 	Missle
		Type	Local 
		At	Windshield
		  Altpiv  5
		#Part1	:MissleSmoke.part
	End

	Event
		EName 	Missle
		Type	Local 
		At	Windshield
		  Altpiv  6
		#Part1	:MissleSmoke.part
	End

#################################################################
	
	Event
		EName 	Thruster1
		Type	Local 
		At	FootR
		Part1	:ThrusterGlowBlue.part
		Part2	:ThrusterCenterBlue.part
		Part3	:Thrustercore.part
		Part4	:ThrusterTrail.part
		Part5	:ThrusterGases.part
	End

	Event
		EName 	Thruster2
		Type	Local 
		At	FootL
		Part1	:ThrusterGlowBlue.part
		Part2	:ThrusterCenterBlue.part
		Part3	:Thrustercore.part
		Part4	:ThrusterTrail.part
		Part5	:ThrusterGases.part
	End	

End

Condition
	On 	Time
	Time 	10

	Event
		EName 	LightTop1
		Type	Local 
		At	T1_L
		Part1	:LightGlowBlue.part
		Part2	:LightCenterBlue.part
		
	End

	Event
		EName 	LightTop2
		Type	Local 
		At	T1_R
		Part1	:LightGlowBlue.part
		Part2	:LightCenterBlue.part
	End

End

Condition
	On 	Time
	Time 	0

	Event
		EName 	LightTop3
		Type	Local 
		At	T2_L
		Part1	:LightGlowRed.part
		Part2	:LightCenterRed.part
	End

	Event
		EName 	LightTop4
		Type	Local 
		At	T2_R
		Part1	:LightGlowRed.part
		Part2	:LightCenterRed.part
	End

	Event
		EName 	LightTop5
		Type	Local 
		At	T3_R
		Part1	:LightGlowRed.part
		Part2	:LightCenterRed.part
	End


############################################################
##Lights
##############################################################
	
	Event
		EName 	LeftSideLT
		Type	local
		At	RingL
		Part1	:WingLightStrip2.part
		PMagnet ToeL

	End

	Event
		EName 	LeftSideLB
		Type	local
		At	ToeL
				
	End


	Event
		EName 	LeftSideRT
		Type	local
		At	F1_L
		Part1	:WingLightStrip2.part
		PMagnet F2_L
	End

	Event
		EName 	LeftSideRB
		Type	local
		At	F2_L
				
	End



################################################


	Event
		EName 	RightSideLT
		Type	local
		At	F1_R
		Part1	:WingLightStrip2.part
		PMagnet F2_R
	End

	Event
		EName 	RightSideLB
		Type	local
		At	F2_R
		
	End


	Event
		EName 	RightSideRT
		Type	local
		At	RingR
		Part1	:WingLightStrip2.part
		PMagnet ToeR
	End

	Event
		EName 	RightSideRB
		Type	local
		At	ToeR
		
	End



End

End

