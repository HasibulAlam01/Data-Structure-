## MindCare System – Overview

The MindCare System is a terminal-based application developed for the Data Structures Lab Assignment. It focuses on mental health assessment and basic patient management in a simple and practical way.

The system allows patients to evaluate their condition through a structured questionnaire covering stress, anxiety, sleep, and depression. Based on the responses, scores are calculated automatically. Doctors can monitor patient data by viewing all records or searching for specific individuals, and they can analyze results to classify risk levels as Low, Moderate, High Risk, or Critical.

This project demonstrates the effective use of multiple data structures. An array is used to store patient assessment results for fast and simple access. A stack is implemented to support undo functionality, allowing the system to remove the most recent record using the LIFO principle. A queue is used to manage appointment requests following the FIFO principle, ensuring fair processing of patients.

This combination of data structures makes the system efficient, organized, and suitable for real-life scenarios while fulfilling the assignment requirements.

However, the system has some limitations. It uses a fixed-size array, so storage is limited. There is no file handling, meaning data is not saved permanently, and the interface is terminal-based only.

In the future, the system can be improved by replacing arrays with linked lists for dynamic storage, adding file handling for data persistence, using hash tables for faster searching, and developing a graphical user interface.

Overall, the MindCare System demonstrates how core data structures can be applied to solve real-world problems such as mental health tracking and appointment management, providing a strong foundation for more advanced Systems.

