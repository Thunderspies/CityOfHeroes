#########################################################
##	
##

FxInfo
#LifeSpan  90

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
	On Time
	Time 0

	Event
		Type	positonly
		At	Fore_ToeR 
		childfx	:bot_activate_Sweeper.fx
		lookat	Fore_FootR  
		bhvroverride
			stretch 1
		end
	End
	Event
		Type	positonly
		At	Fore_FootR  
		childfx	:bot_activate_Sweeper.fx
		lookat	Fore_LLegR 
		bhvroverride
			stretch 1
		end
	End
	Event
		Type	positonly
		At	Fore_LLegR  
		childfx	:bot_activate_Sweeper.fx
		lookat	ClawR 
		bhvroverride
			stretch 1
		end
	End
	Event
		Type	positonly
		At	ClawR  
		childfx	:bot_activate_Sweeper.fx
		lookat	Fore_ULegR 
		bhvroverride
			stretch 1
		end
	End
############## 
	Event
		Type	positonly
		At	HandR 
		childfx	:bot_activate_Sweeper.fx
		lookat	LarmR 
		bhvroverride
			stretch 1
		end
	End
	Event
		Type	positonly
		At	LarmR 
		childfx	:bot_activate_Sweeper.fx
		lookat	UarmR 
		bhvroverride
			stretch 1
		end
	End
	Event
		Type	positonly
		At	UarmR 
		childfx	:bot_activate_Sweeper.fx
		lookat	RingR 
		bhvroverride
			stretch 1
		end
	End
	Event
		Type	positonly
		At	RingR 
		childfx	:bot_activate_Sweeper.fx
		lookat	Col_R 
		bhvroverride
			stretch 1
		end
	End

##############
	Event
		Type	positonly
		At	Rwing4 
		childfx	:bot_activate_Sweeper.fx
		lookat	Rwing3 
		bhvroverride
			stretch 1
		end
	End
	Event
		Type	positonly
		At	Rwing3  
		childfx	:bot_activate_Sweeper.fx
		lookat	Rwing2 
		bhvroverride
			stretch 1
		end
	End
	Event
		Type	positonly
		At	Rwing2  
		childfx	:bot_activate_Sweeper.fx
		lookat	SpadR 
		bhvroverride
			stretch 1
		end
	End
	Event
		Type	positonly
		At	SpadR  
		childfx	:bot_activate_Sweeper.fx
		lookat	Rwing1 
		bhvroverride
			stretch 1
		end
	End
############## 
	Event
		Type	positonly
		At	ToeR 
		childfx	:bot_activate_Sweeper.fx
		lookat	FootR
		bhvroverride
			stretch 1
		end
	End
	Event
		Type	positonly
		At	FootR 
		childfx	:bot_activate_Sweeper.fx
		lookat	LlegR 
		bhvroverride
			stretch 1
		end
	End
	Event
		Type	positonly
		At	LlegR 
		childfx	:bot_activate_Sweeper.fx
		lookat	BootR 
		bhvroverride
			stretch 1
		end
	End
	Event
		Type	positonly
		At	BootR 
		childfx	:bot_activate_Sweeper.fx
		lookat	UlegR 
		bhvroverride
			stretch 1
		end
	End


end
