#########################################################
##	template

FxInfo



#########################################################
####################### AUDIO ###########################
#########################################################
Condition
	On	Time
	Time	0
	

	Event
		Type	Local
		At 	Origin
		Sound terror2 80 80 .7
	End
End


Condition
	On	Time
	Time	0
	

	Event
		Type	Local
		At 	Origin
		Sound terror2_loop 80 80 .4
	End
End




#########################################################
#################### BODY GLOW 1 ########################
#########################################################



Condition
	On	Time
	Time	30
	
	Event	
		ENAME	ChestGlowMain
		Type	Local
		At	Neck
		part	:DarkIllusTerrorChestGlow01.part
		#part	:DarkIllusTerrorRays01.part
		part	:DarkIllusTerrorRisingGlows01.part
		part	:DarkIllusTerrorGlowSubtle.part

		POther  Hips
	
	End

	Event	
		ENAME	RightChestGlow
		Type	Local
		At	UarmR
		part	:DarkIllusTerrorGlow01.part
		part	:DarkIllusTerrorBodyGlow01.part
		part	:DarkIllusTerrorRays01.part
		part	:DarkIllusTerrorGlowSubtle.part
		POther  Hips
		#Bhvr	Behaviors/TelportationFade.bhvr
		
		
	End

	Event	
		ENAME	LeftChestGlow
		Type	Local
		At	UarmL
		part	:DarkIllusTerrorGlow01.part
		part	:DarkIllusTerrorBodyGlow01.part
		part	:DarkIllusTerrorRays01.part
		part	:DarkIllusTerrorGlowSubtle.part
		POther  Hips
		#Bhvr	Behaviors/TelportationFade.bhvr
		
		
	End
	
	Event	
		ENAME	Face
		Type	Local
		At	Eyes
		
		BhvrOverride
			PositionOffset	-.3 -.1 -.3
		End

		Part	:DarkIllusTerrorFace.part
		part	:DarkIllusTerrorFace01.part
		part	:DarkIllusTerrorRisingGlows01.part
		part	:DarkIllusTerrorGlowSubtle.part
		POther  Chest
		#Bhvr	Behaviors/TelportationFade.bhvr
		
		
	End

	Event	
		ENAME	RightUarmGlow
		Type	Local
		At	SpadR
		part	:DarkIllusTerrorGlow01.part
		part	:DarkIllusTerrorRisingGlows01.part
		part	:DarkIllusTerrorBodyGlow01.part
		part	:DarkIllusTerrorGlowSubtle.part
		POther  LarmR
		#Bhvr	Behaviors/TelportationFade.bhvr	
		
	End

	Event	
		ENAME	LeftUarmGlow
		Type	Local
		At	SpadL
		part	:DarkIllusTerrorGlow01.part
		part	:DarkIllusTerrorRisingGlows01.part
		part	:DarkIllusTerrorBodyGlow01.part
		part	:DarkIllusTerrorGlowSubtle.part
		POther  LarmL
		#Bhvr	Behaviors/TelportationFade.bhvr
		
		
	End

	Event	
		ENAME	RightLarmGlow
		Type	Local
		At	HandR
		part	:DarkIllusTerrorGlow01.part
		part	:DarkIllusTerrorRisingGlows01.part
		part	:DarkIllusTerrorBodyGlow01.part
		part	:DarkIllusTerrorGlowSubtle.part
		POther  LarmR
		#Bhvr	Behaviors/TelportationFade.bhvr
		
		
	End

	
	Event	
		ENAME	LeftLarmGlow
		Type	Local
		At	HandL
		part	:DarkIllusTerrorGlow01.part
		part	:DarkIllusTerrorRisingGlows01.part
		part	:DarkIllusTerrorBodyGlow01.part
		part	:DarkIllusTerrorGlowSubtle.part
		POther  LarmL
		#Bhvr	Behaviors/TelportationFade.bhvr
	
	End

	Event	
		ENAME	LeftHandGlow
		Type	Local
		At	HandL
		part	:DarkIllusTerrorWristGlow01.part
		POther  RingL
		#Bhvr	Behaviors/TelportationFade.bhvr

	End

	Event	
		ENAME	RightHandGlow
		Type	Local
		At	HandR
		part	:DarkIllusTerrorWristGlow01.part
		POther  RingR
		#Bhvr	Behaviors/TelportationFade.bhvr
		
	End

	
############################## OLD HANDS ##############################
	Event	
		ENAME	HandLeft
		Type	Local
		At	HandL
		Part	:DarkIllusTerrorHand02.part
		Part	:DarkIllusTerrorHandDarkl.part
		#Bhvr	Behaviors/TelportationFade.bhvr

		
	End

	Event	
		ENAME	HandRight
		Type	Local
		At	HandR
		Part	:DarkIllusTerrorHand02.part
		Part	:DarkIllusTerrorHandDarkl.part
		#Bhvr	Behaviors/TelportationFade.bhvr
		
	End

End

############################## SUMMON ##############################
Condition
	On	Time
	Time	0
	
	Event	
		ENAME	ChestMagnet
		Type	Start
		At	Chest
		#Geom	Junk_Car_01
		part	:DarkIllusTerrorSummonCloudIn.part
		part	:DarkIllusTerrorSummonGlows.part
		Lifespan 35
	
	End

End

Condition
	On	Time
	Time	35
	
	Event	
		ENAME	ChestMagnet
		Type	Start
		At	Head
		#Geom	Junk_Car_01
		part	:DarkIllusTerrorSummonGlowStatic.part
		part	:DarkIllusTerrorSummonRays01.part
		Lifespan 15
	
	End

End

End
##################################

